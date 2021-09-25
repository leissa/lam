#include "lexer.h"

#include <iostream>

namespace lam {

Lexer::Lexer(const char* filename, std::istream& stream)
    : loc_{filename, {1, 1}, {1, 1}}
    , peek_pos_({1, 1})
    , stream_(stream)
{
    if (!stream_) throw std::runtime_error("stream is bad");
}

int Lexer::next() {
    loc_.finis = peek_pos_;
    int c = stream_.get();

    if (c == '\n') {
        ++peek_pos_.row;
        peek_pos_.col = 1;
    } else {
        ++peek_pos_.col;
    }

    return c;
}

Tok Lexer::lex() {
    while (true) {
        loc_.begin = peek_pos_;
        str_.clear();

        if (eof()) return tok(Tok::Tag::EoF);
        if (accept_if(isspace)) continue;
        if (accept('=')) return tok(Tok::Tag::Assign);
        if (accept('.')) return tok(Tok::Tag::Dot);
        if (accept('(')) return tok(Tok::Tag::Paren_L);
        if (accept(')')) return tok(Tok::Tag::Paren_R);
        if (accept('/')) {
            if (accept('*')) {
                eat_comments();
                continue;
            }
            if (accept('/')) {
                while (!eof() && peek() != '\n') next();
                continue;
            }

            std::cerr << Loc(loc_.file, peek_pos_) << ": invalid input char '/'; maybe you wanted to start a comment?" << std::endl;
            continue;
        }

        // lex identifier or keyword
        if (accept_if([](int i) { return i == '_' || isalpha(i); })) {
            while (accept_if([](int i) { return i == '_' || isalpha(i) || isdigit(i); })) {}
            if (str_ == "in" ) return tok(Tok::Tag::In);
            if (str_ == "lam") return tok(Tok::Tag::Lam);
            if (str_ == "let") return tok(Tok::Tag::Let);
            std::cerr << loc() << ": " << str_ << std::endl;
            return tok(str_.c_str());
        }

        std::cerr << Loc(loc_.file, peek_pos_) << ": invalid input char: '" << (char) peek() << "'" << std::endl;
        next();
    }
}

void Lexer::eat_comments() {
    while (true) {
        while (!eof() && peek() != '*') next();
        if (eof()) {
            std::cerr << loc_ << ": non-terminated multiline comment" << std::endl;
            return;
        }
        next();
        if (accept('/')) break;
    }
}

}
