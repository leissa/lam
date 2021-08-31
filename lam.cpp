#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

template<class T>
using Ptr = std::unique_ptr<T>;

template<class T, class... Args>
Ptr<T> mk(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

class Exp {
public:
    virtual ~Exp() {}

    void dump() const {
        stream(std::cout) << std::endl;
    }

    virtual Ptr<Exp> clone() const = 0;
    virtual std::ostream& stream(std::ostream&) const = 0;
    virtual Ptr<Exp> rename(const std::string&, const std::string&) const = 0;
    virtual Ptr<Exp> subst(const std::string&, const Exp&) const = 0;
    virtual Ptr<Exp> eval() const = 0;
};

class Var : public Exp {
public:
    Var(const std::string& name)
        : name_(std::move(name))
    {}

    const std::string& name() const { return name_; }

    Ptr<Exp> clone() const override { return mk<Var>(name()); }
    Ptr<Exp> rename(const std::string& x, const std::string& y) const override;
    Ptr<Exp> subst(const std::string& x, const Exp& e) const;
    Ptr<Exp> eval() const override;
    std::ostream& stream(std::ostream& o) const override;

private:
    std::string name_;
};

class App : public Exp {
public:
    App(Ptr<Exp>&& callee, Ptr<Exp>&& arg)
        : callee_(std::move(callee))
        , arg_(std::move(arg))
    {}

    const Exp* callee() const { return callee_.get(); }
    const Exp* arg() const { return arg_.get(); }

    Ptr<Exp> clone() const override { return mk<App>(callee()->clone(), arg()->clone()); }
    Ptr<Exp> rename(const std::string&, const std::string& ) const override;
    Ptr<Exp> subst(const std::string&, const Exp&) const override;
    Ptr<Exp> eval() const override;
    std::ostream& stream(std::ostream&) const override;

private:
    Ptr<Exp> callee_;
    Ptr<Exp> arg_;
};

class Lam : public Exp {
public:
    Lam(const std::string& binder, Ptr<Exp>&& body)
        : binder_(std::move(binder))
        , body_(std::move(body))
    {}

    const std::string& binder() const { return binder_; }
    const Exp* body() const { return body_.get(); }

    Ptr<Exp> clone() const override { return mk<Lam>(binder(), body()->clone()); }
    Ptr<Exp> rename(const std::string& x, const std::string& y) const override;
    Ptr<Exp> subst(const std::string& x, const Exp& e) const override;
    Ptr<Exp> eval() const override;
    std::ostream& stream(std::ostream& o) const override;

private:
    static int counter_;

    std::string binder_;
    Ptr<Exp> body_;
};

int Lam::counter_ = 0;

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
    if (binder() == x) return body()->clone();

    auto fresh = binder() + "_" + std::to_string(counter_++);
    auto b = body()->rename(binder(), fresh);
    return mk<Lam>(fresh, b->subst(x, e));
}

// eval

Ptr<Exp> Var::eval() const {
    return clone();
}

Ptr<Exp> App::eval() const {
    auto c = callee()->eval();
    auto a = arg()->eval();

    if (auto lam = dynamic_cast<const Lam*>(c.get()))
        return lam->body()->subst(lam->binder(), *a);

    return mk<App>(std::move(c), std::move(a));
}

Ptr<Exp> Lam::eval() const {
    return mk<Lam>(binder(), body()->eval());
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

// driver

int main() {
    auto p = mk<Var>("x");
    auto x = p->eval();
    //x->dump();
    // (lam x. x) z
    auto p1 = mk<App>(mk<Lam>("x", mk<Var>("x")), mk<Var>("z"));
    p1->dump();

    auto p2 = p1->clone();
    p1->dump();
    p1.reset();
    p2->dump();

    // λf. λx. x
    auto n0 = mk<Lam>("f", mk<Lam>("x", mk<Var>("x")));
    // λf. λx. f x
    auto n1 = mk<Lam>("f", mk<Lam>("x", mk<App>(mk<Var>("f"), mk<Var>("x"))));
    // λf. λx. f (f x)
    auto n2 = mk<Lam>("f", mk<Lam>("x", mk<App>(mk<Var>("f"), mk<App>(mk<Var>("f"), mk<Var>("x")))));

    n0->dump();
    n1->dump();
    n2->dump();

    // λn. λf. λx. f (n f x)
    auto succ = mk<Lam>("n", mk<Lam>("f", mk<Lam>("x", mk<App>(mk<Var>("f"), mk<App>(mk<App>(mk<Var>("n"), mk<Var>("f")), mk<Var>("x"))))));
    succ->dump();

    mk<App>(succ->clone(), n1->clone())->eval()->dump();
    n2->dump();

    auto p3 = p2->eval();
    //p3->dump();

    return 0;
}
