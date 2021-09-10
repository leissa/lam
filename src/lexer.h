#ifndef LAM_LEXER_H
#define LAM_LEXER_H

#include <cassert>
#include <istream>

#include "loc.h"


namespace lam {

class Tok {
public:
    enum class Tag {
        EoF, Id, Lam, Dot, Paren_L, Paren_R
    };

    Tok() {}
    Tok(Tag tag)
        : tag_(tag)
    {}
    Tok(const std::string& str)
        : tag_(Tag::Id)
        , str_(str)
    {}

    Tag tag() const { return tag_; }
    bool isa(Tag tag) const { return tag == tag_; }
    const std::string& str() const { assert(isa(Tag::Id)); return str_; }

private:
    Tag tag_;
    std::string str_;
};

class Lexer {
public:
    Lexer(const char*, std::istream&);

    Tok lex(); ///< Get next @p Tok in stream.

private:
    bool eof() const { peek(); return stream_.eof(); }

    template <typename Pred>
    bool accept_if(Pred pred) {
        if (pred(peek())) {
            str_ += next();
            return true;
        }
        return false;
    }

    bool accept(int val) {
        return accept_if([val] (int p) { return p == val; });
    }

    int next();
    int peek() const { return stream_.peek(); }

    Loc loc_;
    std::istream& stream_;
    std::string str_;
};

}

#endif

