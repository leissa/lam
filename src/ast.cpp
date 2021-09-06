#include "ast.h"

#include <iostream>

namespace lam {

int Lam::counter_ = 0;

// clone

Ptr<Exp> Var::clone() const {
    return mk<Var>(name());
}

Ptr<Exp> App::clone() const {
    return mk<App>(callee()->clone(), arg()->clone());
}

Ptr<Exp> Lam::clone() const {
    return mk<Lam>(binder(), body()->clone());
}

// stream

std::ostream& Var::stream(std::ostream& o) const {
    return o << name();
}

std::ostream& App::stream(std::ostream& o) const {
    o << "(";
    callee()->stream(o) << " ";
    return arg()->stream(o) << ")";
}

std::ostream& Lam::stream(std::ostream& o) const {
    o << "(λ " << binder() << ". ";
    return body()->stream(o) << ")";
}

void Exp::dump() const {
    stream(std::cout) << std::endl;
}

// free_vars

Vars Var::free_vars() const {
    return Vars({name()});
}

Vars App::free_vars() const {
    auto c = callee()->free_vars();
    auto a = callee()->free_vars();
    c.insert(a.begin(), a.end());
    return c;
}

Vars Lam::free_vars() const {
    auto res = body()->free_vars();
    res.erase(binder());
    return res;
}

// rename

Ptr<Exp> Var::rename(const std::string& x, const std::string& y) const {
    return name() == x ? mk<Var>(y) : clone();
}

Ptr<Exp> App::rename(const std::string& x, const std::string& y) const {
    auto c = callee()->rename(x, y);
    auto a = arg()->rename(x, y);
    return mk<App>(std::move(c), std::move(a));
}

Ptr<Exp> Lam::rename(const std::string& x, const std::string& y) const {
    return mk<Lam>(binder() == x ? y : binder(), body()->rename(x, y));
}

// subst

Ptr<Exp> Var::subst(const std::string& x, const Exp& e) const {
    return name() == x ? e.clone() : clone();
}

Ptr<Exp> App::subst(const std::string& x, const Exp& e) const {
    auto c = callee()->subst(x, e);
    auto a = arg()->subst(x, e);
    return mk<App>(std::move(c), std::move(a));
}

Ptr<Exp> Lam::subst(const std::string& x, const Exp& e) const {
    if (binder() == x) return mk<Lam>(binder(), body()->clone());
    if (e.free_vars().count(binder()) == 0) return mk<Lam>(binder(), body()->subst(x, e));

    auto fresh = binder() + "_" + std::to_string(counter_++);
    return mk<Lam>(fresh, body()->rename(binder(), fresh)->subst(x, e));
}

// eval

Ptr<Exp> Var::eval() const {
    return clone();
}

Ptr<Exp> App::eval() const {
    auto c = callee()->eval();
    auto a = arg()->eval();

    if (auto lam = dynamic_cast<const Lam*>(c.get()))
        return lam->body()->subst(lam->binder(), *a)->eval();

    return mk<App>(std::move(c), std::move(a));
}

Ptr<Exp> Lam::eval() const {
    return mk<Lam>(binder(), body()->eval());
}

}
