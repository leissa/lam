#ifndef LAM_AST_H
#define LAM_AST_H

#include <memory>
#include <ostream>
#include <unordered_set>

namespace lam {

template<class T>
using Ptr = std::unique_ptr<T>;

template<class T, class... Args>
Ptr<T> mk(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

using Vars = std::unordered_set<std::string>;

class Exp {
public:
    virtual ~Exp() {}

    void dump() const;

    virtual Ptr<Exp> clone() const = 0;
    virtual std::ostream& stream(std::ostream&) const = 0;
    virtual Vars free_vars() const = 0;
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

    Ptr<Exp> clone() const override;
    std::ostream& stream(std::ostream& o) const override;
    Vars free_vars() const override;
    Ptr<Exp> rename(const std::string& x, const std::string& y) const override;
    Ptr<Exp> subst(const std::string& x, const Exp& e) const override;
    Ptr<Exp> eval() const override;

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

    Ptr<Exp> clone() const override;
    std::ostream& stream(std::ostream&) const override;
    Vars free_vars() const override;
    Ptr<Exp> rename(const std::string&, const std::string& ) const override;
    Ptr<Exp> subst(const std::string&, const Exp&) const override;
    Ptr<Exp> eval() const override;

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

    Ptr<Exp> clone() const override;
    std::ostream& stream(std::ostream& o) const override;
    Vars free_vars() const override;
    Ptr<Exp> rename(const std::string& x, const std::string& y) const override;
    Ptr<Exp> subst(const std::string& x, const Exp& e) const override;
    Ptr<Exp> eval() const override;

private:
    static int counter_;

    std::string binder_;
    Ptr<Exp> body_;
};

}

#endif
