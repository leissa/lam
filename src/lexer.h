#ifndef LAM_LEXER_H
#define LAM_LEXER_H

#include <cassert>
#include <istream>

#include "tok.h"

namespace lam {

class Lexer {
public:
    Lexer(const char*, std::istream&);

    Loc loc() const { return loc_; }
    Tok lex(); ///< Get next @p Tok in stream.

private:
    Tok tok(Tok::Tag tag) { return {loc(), tag}; }
    Tok tok(const char* str) { return {loc(), str}; }
    bool eof() const { peek(); return stream_.eof(); }

    template<class Pred>
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

    Loc loc_; ///< @p Loc%ation of the @p Tok%en we are currently constructing within @p str_,
    Pos pos_; ///< @p Pos%ition of the current @p peek().
    std::istream& stream_;
    std::string str_;
};

}

#endif

