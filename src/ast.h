#ifndef LAM_AST_H
#define LAM_AST_H

#include <memory>
#include <ostream>
#include <unordered_set>

#include "loc.h"

namespace lam {

template<class T>
using Ptr = std::unique_ptr<T>;

template<class T, class... Args>
Ptr<T> mk(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

using Vars = std::unordered_set<std::string>;

class Exp {
public:
    Exp(Loc loc)
        : loc_(loc)
    {}

    virtual ~Exp() {}

    Loc loc() const { return loc_; }
    void dump() const;
    Vars free_vars() const;

    virtual Ptr<Exp> clone() const = 0;
    virtual std::ostream& stream(std::ostream&) const = 0;
    virtual void free_vars(Vars&) const = 0;
    virtual Ptr<Exp> rename(const std::string&, const std::string&) const = 0;
    virtual Ptr<Exp> subst(const std::string&, const Exp&) const = 0;
    virtual Ptr<Exp> eval(bool) const = 0;

private:
    Loc loc_;
};

class Var : public Exp {
public:
    Var(Loc loc, const std::string& name)
        : Exp(loc)
        , name_(std::move(name))
    {}

    const std::string& name() const { return name_; }

    Ptr<Exp> clone() const override;
    std::ostream& stream(std::ostream& o) const override;
    void free_vars(Vars&) const override;
    Ptr<Exp> rename(const std::string&, const std::string&) const override;
    Ptr<Exp> subst(const std::string&, const Exp&) const override;
    Ptr<Exp> eval(bool) const override;

private:
    std::string name_;
};

class App : public Exp {
public:
    App(Loc loc, Ptr<Exp>&& callee, Ptr<Exp>&& arg)
        : Exp(loc)
        , callee_(std::move(callee))
        , arg_(std::move(arg))
    {}

    const Exp* callee() const { return callee_.get(); }
    const Exp* arg() const { return arg_.get(); }

    Ptr<Exp> clone() const override;
    std::ostream& stream(std::ostream&) const override;
    void free_vars(Vars&) const override;
    Ptr<Exp> rename(const std::string&, const std::string&) const override;
    Ptr<Exp> subst(const std::string&, const Exp&) const override;
    Ptr<Exp> eval(bool) const override;

private:
    Ptr<Exp> callee_;
    Ptr<Exp> arg_;
};

class Lam : public Exp {
public:
    Lam(Loc loc, const std::string& binder, Ptr<Exp>&& body)
        : Exp(loc)
        , binder_(std::move(binder))
        , body_(std::move(body))
    {}

    const std::string& binder() const { return binder_; }
    const Exp* body() const { return body_.get(); }

    Ptr<Exp> clone() const override;
    std::ostream& stream(std::ostream& o) const override;
    void free_vars(Vars&) const override;
    Ptr<Exp> rename(const std::string&, const std::string&) const override;
    Ptr<Exp> subst(const std::string&, const Exp&) const override;
    Ptr<Exp> eval(bool) const override;

private:
    static int counter_;

    std::string binder_;
    Ptr<Exp> body_;
};

class Err : public Exp {
public:
    Err(Loc loc)
        : Exp(loc)
    {}

    Ptr<Exp> clone() const override;
    std::ostream& stream(std::ostream& o) const override;
    void free_vars(Vars&) const override;
    Ptr<Exp> rename(const std::string&, const std::string&) const override;
    Ptr<Exp> subst(const std::string&, const Exp&) const override;
    Ptr<Exp> eval(bool) const override;

private:
    std::string name_;
};

}

#endif
