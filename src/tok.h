#ifndef LAM_TOK_H
#define LAM_TOK_H

#include <cassert>

#include "loc.h"
#include "sym.h"

namespace lam {

#define LAM_KEYWORDS(m) \
    m(K_in,  "in" )     \
    m(K_lam, "lam")     \
    m(K_let, "let")

#define LAM_PUNCTUATORS(m)  \
    m(P_assign,  "=")       \
    m(P_dot,     ".")       \
    m(P_paren_l, "(")       \
    m(P_paren_r, ")")

#define LAM_MISC(m)  \
    m(M_eof, "<end of file>") \
    m(M_id,  "<identifier>")

class Tok {
public:
    enum class Tag {
#define CODE(t, str) t,
        LAM_KEYWORDS(CODE)
        LAM_PUNCTUATORS(CODE)
        LAM_MISC(CODE)
#undef CODE
    };

    Tok() {}
    Tok(Loc loc, Tag tag)
        : loc_(loc)
        , tag_(tag)
    {}
    Tok(Loc loc, Sym sym)
        : loc_(loc)
        , tag_(Tag::M_id)
        , sym_(sym)
    {}

    Loc loc() const { return loc_; }
    Tag tag() const { return tag_; }
    bool isa(Tag tag) const { return tag == tag_; }
    Sym sym() const { assert(isa(Tag::M_id)); return sym_; }
    static const char* tag2str(Tok::Tag);

private:
    Loc loc_;
    Tag tag_;
    Sym sym_;
};

std::ostream& operator<<(std::ostream&, const Tok&);

}

#endif
