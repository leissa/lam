#include "lexer.h"

#include <iostream>

namespace lam {

Lexer::Lexer(const char* filename, std::istream& stream)
    : loc_{filename, {1, 1}, {1, 1}}
    , pos_({1, 1})
    , stream_(stream)
{
    if (!stream_) throw std::runtime_error("stream is bad");
}

int Lexer::next() {
    loc_.finis = pos_;
    ++pos_.col;
    return stream_.get();
}

Tok Lexer::lex() {
    while (true) {
        loc_.begin = pos_;
        str_.clear();

        if (eof()) return tok(Tok::Tag::EoF);

        if (accept('\n')) {
            accept('\r'); // DOS newline
            ++pos_.row;
            pos_.col = 1;
            continue;
        }

        if (accept_if(isspace)) continue;
        if (accept('.')) return tok(Tok::Tag::Dot);
        if (accept('(')) return tok(Tok::Tag::Paren_L);
        if (accept(')')) return tok(Tok::Tag::Paren_R);

        if (accept_if(isalpha)) {
            while (accept_if([](int i) { return isalpha(i) || isdigit(i); })) {}
            if (str_ == "lam") return tok(Tok::Tag::Lam);
            return tok(str_.c_str());
        }

        std::cerr << loc_<< ": invalid input char: '" << (char) peek() << "'" << std::endl;
        next();
    }
}

}
