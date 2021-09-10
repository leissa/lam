#include "lexer.h"

#include <iostream>

namespace lam {

Lexer::Lexer(const char* filename, std::istream& stream)
    : loc{{1, 1}, {1, 1}, filename}
    , stream_(stream)
{
    if (!stream_) throw std::runtime_error("stream is bad");
}

int Lexer::next() {
    return stream_.get();
}

Tok Lexer::lex() {
    while (true) {
        str_.clear();

        if (eof()) return Tok(Tok::Tag::EoF);
        if (accept('\n')) {
            accept('\r');
            ++loc.begin.row;
            loc.begin.col = 1;
            loc.finis = loc.begin;
            continue;
        }

        if (accept_if(isspace)) continue;
        if (accept('.')) return Tok(Tok::Tag::Dot);
        if (accept('(')) return Tok(Tok::Tag::Paren_L);
        if (accept(')')) return Tok(Tok::Tag::Paren_R);

        if (accept_if(isalpha)) {
            while (accept_if([](int i) { return isalpha(i) || isdigit(i); })) {}
            if (str_ == "lam") return Tok(Tok::Tag::Lam);
            return Tok(str_.c_str());
        }

        std::cerr << "invalid input char: '" << peek() << "'" << std::endl;
        next();
    }
}

}
