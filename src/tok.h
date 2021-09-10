#ifndef LAM_TOK_H
#define LAM_TOK_H

#include <cassert>

#include "loc.h"

namespace lam {

class Tok {
public:
    enum class Tag {
        EoF, Id, Lam, Dot, Paren_L, Paren_R
    };

    Tok() {}
    Tok(Loc loc, Tag tag)
        : loc_(loc)
        , tag_(tag)
    {}
    Tok(Loc loc, const std::string& str)
        : loc_(loc)
        , tag_(Tag::Id)
        , str_(str)
    {}

    Loc loc() const { return loc_; }
    Tag tag() const { return tag_; }
    bool isa(Tag tag) const { return tag == tag_; }
    const std::string& str() const { assert(isa(Tag::Id)); return str_; }
    static const char* tag2str(Tok::Tag);

private:
    Loc loc_;
    Tag tag_;
    std::string str_;
};

std::ostream& operator<<(std::ostream&, const Tok&);

}

#endif
