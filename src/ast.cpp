#include "ast.h"

#include <iostream>

namespace lam {

int Lam::counter_ = 0;

// clone

Ptr<Exp> Var::clone() const {
    return mk<Var>(loc(), name());
}

Ptr<Exp> App::clone() const {
    return mk<App>(loc(), callee()->clone(), arg()->clone());
}

Ptr<Exp> Lam::clone() const {
    return mk<Lam>(loc(), binder(), body()->clone());
}

Ptr<Exp> Err::clone() const {
    return mk<Err>(loc());
}

// stream

void Exp::dump() const {
    stream(std::cout) << std::endl;
}

std::ostream& Var::stream(std::ostream& o) const {
    return o << name();
}

std::ostream& App::stream(std::ostream& o) const {
    o << "(";
    callee()->stream(o) << " ";
    return arg()->stream(o) << ")";
}

std::ostream& Lam::stream(std::ostream& o) const {
    o << "(lam " << binder() << ". ";
    return body()->stream(o) << ")";
}

std::ostream& Err::stream(std::ostream& o) const {
    return o << "<error>";
}

// free_vars

Vars Exp::free_vars() const {
    Vars vars;
    free_vars(vars);
    return vars;
}

void Var::free_vars(Vars& vars) const {
    vars.emplace(name());
}

void App::free_vars(Vars& vars) const {
    callee()->free_vars(vars);
    arg()->free_vars(vars);
}

void Lam::free_vars(Vars& vars) const {
    body()->free_vars(vars);
    vars.erase(binder());
}

void Err::free_vars(Vars&) const {}

// rename

Ptr<Exp> Var::rename(const std::string& x, const std::string& y) const {
    return name() == x ? mk<Var>(loc(), y) : clone();
}

Ptr<Exp> App::rename(const std::string& x, const std::string& y) const {
    auto c = callee()->rename(x, y);
    auto a = arg()->rename(x, y);
    return mk<App>(loc(), std::move(c), std::move(a));
}

Ptr<Exp> Lam::rename(const std::string& x, const std::string& y) const {
    return mk<Lam>(loc(), binder() == x ? y : binder(), body()->rename(x, y));
}

Ptr<Exp> Err::rename(const std::string&, const std::string&) const {
    return clone();
}

// subst

Ptr<Exp> Var::subst(const std::string& x, const Exp& e) const {
    return name() == x ? e.clone() : clone();
}

Ptr<Exp> App::subst(const std::string& x, const Exp& e) const {
    auto c = callee()->subst(x, e);
    auto a = arg()->subst(x, e);
    return mk<App>(loc(), std::move(c), std::move(a));
}

Ptr<Exp> Lam::subst(const std::string& x, const Exp& e) const {
    if (binder() == x) return mk<Lam>(loc(), binder(), body()->clone());
    if (e.free_vars().count(binder()) == 0) return mk<Lam>(loc(), binder(), body()->subst(x, e));

    auto fresh = binder() + "_" + std::to_string(counter_++);
    return mk<Lam>(loc(), fresh, body()->rename(binder(), fresh)->subst(x, e));
}

Ptr<Exp> Err::subst(const std::string&, const Exp&) const {
    return clone();
}

// eval

Ptr<Exp> Var::eval(bool) const {
    return clone();
}

Ptr<Exp> App::eval(bool stop_on_lam) const {
    auto c = callee()->eval(true);

    if (auto lam = dynamic_cast<const Lam*>(c.get()))
        return lam->body()->subst(lam->binder(), *arg())->eval(stop_on_lam);

    return mk<App>(loc(), std::move(c), arg()->eval());
}

Ptr<Exp> Lam::eval(bool stop_on_lam) const {
    return stop_on_lam ? clone() : mk<Lam>(loc(), binder(), body()->eval());
}

Ptr<Exp> Err::eval(bool) const {
    return clone();
}

}
