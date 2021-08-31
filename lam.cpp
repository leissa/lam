#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

/*
 * e := x
 *   | e e
 *   | lam x. exp
 */

class Exp;
using Ctxt = std::unordered_map<std::string, std::unique_ptr<Exp>>;

class Exp {
public:
    virtual ~Exp() {}

    void dump() const {
        stream(std::cout) << std::endl;
    }

    std::unique_ptr<Exp> run() const {
        Ctxt ctxt;
        return eval(ctxt);
    }

    virtual std::ostream& stream(std::ostream&) const = 0;
    virtual std::unique_ptr<Exp> clone() const = 0;
    virtual std::unique_ptr<Exp> eval(Ctxt&) const = 0;
};

class Lam : public Exp {
public:
    Lam(std::string&& binder, std::unique_ptr<Exp>&& body)
        : binder_(std::move(binder))
        , body_(std::move(body))
    {}

    const std::string& binder() const { return binder_; }
    const Exp* body() const { return body_.get(); }

    std::ostream& stream(std::ostream& o) const override {
        o << "(λ " << binder() << ". ";
        return body()->stream(o) << ")";
    }

    std::unique_ptr<Exp> clone() const override {
        return std::make_unique<Lam>(std::string(binder()), body()->clone());
    }

    std::unique_ptr<Exp> eval(Ctxt& ctxt) const override {
        return std::make_unique<Lam>(std::string(binder()), body()->eval(ctxt));
    }

private:
    static int fresh_ = 0;

    std::string binder_;
    std::unique_ptr<Exp> body_;
};

class Var : public Exp {
public:
    Var(std::string&& name)
        : name_(std::move(name))
    {}

    const std::string& name() const { return name_; }

    std::ostream& stream(std::ostream& o) const override {
        return o << name();
    }

    std::unique_ptr<Exp> clone() const override {
        return std::make_unique<Var>(std::string(name()));
    }

    std::unique_ptr<Exp> eval(Ctxt& ctxt) const override {
        auto i = ctxt.find(name());
        return i != ctxt.end() ? i->second->clone() : clone();
    }

private:
    std::string name_;
};

class App : public Exp {
public:
    App(std::unique_ptr<Exp>&& callee, std::unique_ptr<Exp>&& arg)
        : callee_(std::move(callee))
        , arg_(std::move(arg))
    {}

    const Exp* callee() const { return callee_.get(); }
    const Exp* arg() const { return arg_.get(); }

    std::ostream& stream(std::ostream& o) const override {
        o << "(";
        callee()->stream(o) << " ";
        return arg()->stream(o) << ")";
    }

    std::unique_ptr<Exp> clone() const override {
        return std::make_unique<App>(callee()->clone(), arg()->clone());
    }

    std::unique_ptr<Exp> eval(Ctxt& ctxt) const override {
        auto c = callee()->eval(ctxt);
        auto a = arg()->eval(ctxt);

        if (auto lam = dynamic_cast<const Lam*>(c.get())) {
            ctxt[lam->binder()] = std::move(a);
            return lam->body()->eval(ctxt);
        }

        return std::make_unique<App>(std::move(c), std::move(a));
    }
private:
    std::unique_ptr<Exp> callee_;
    std::unique_ptr<Exp> arg_;
};

auto var(std::string&& x) { return std::make_unique<Var>(std::move(x)); }
auto app(std::unique_ptr<Exp>&& callee, std::unique_ptr<Exp>&& arg) { return std::make_unique<App>(std::move(callee), std::move(arg)); }
auto lam(std::string&& x, std::unique_ptr<Exp>&& body) { return std::make_unique<Lam>(std::move(x), std::move(body)); }

int main() {
    auto p = var("x");
    auto x = p->run();
    //x->dump();
    // (lam x. x) z
    auto p1 = app(lam("x", var("x")), var("z"));
    p1->dump();

    auto p2 = p1->clone();
    p1->dump();
    p1.reset();
    p2->dump();

    // λf. λx. x
    auto n0 = lam("f0", lam("x0", var("x0")));
    // λf. λx. f x
    auto n1 = lam("f1", lam("x1", app(var("f1"), var("x1"))));
    // λf. λx. f (f x)
    auto n2 = lam("f2", lam("x2", app(var("f2"), app(var("f2"), var("x2")))));

    n0->dump();
    n1->dump();
    n2->dump();

    // λn. λf. λx. f (n f x)
    auto succ = lam("n", lam("fs", lam("xs", app(var("fs"), app(app(var("n"), var("fs")), var("xs"))))));
    succ->dump();

    app(succ->clone(), n1->clone())->run()->dump();
    n2->dump();

    auto p3 = p2->run();
    //p3->dump();

    return 0;
}
