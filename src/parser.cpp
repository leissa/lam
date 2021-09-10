#include "parser.h"

#include <fstream>
#include <iostream>

namespace lam {

Parser::Parser(const char* filename, std::istream& stream)
    : lexer_(filename, stream)
    , ahead_(lexer_.lex())
{}

Tok Parser::lex() {
    auto result = ahead();
    ahead_ = lexer_.lex();
    return result;
}

bool Parser::accept(Tok::Tag tag) {
    if (tag != ahead().tag()) return false;
    lex();
    return true;
}

bool Parser::expect(Tok::Tag tag) {
    if (ahead().tag() == tag) {
        lex();
        return true;
    }

    err();
    return false;
}

void Parser::err() {
    std::cerr << "syntax error" << std::endl;
    lex();
}

Ptr<Exp> Parser::parse_exp_() {
    switch (ahead().tag()) {
        case Tok::Tag::Id:  return parse_var();
        case Tok::Tag::Lam: return parse_lam();
        case Tok::Tag::Paren_L: {
            lex();
            auto res = parse_exp();
            expect(Tok::Tag::Paren_R);
            return res;
        }
        default:            return nullptr;
    }
}

Ptr<Exp> Parser::parse_exp() {
    auto l = parse_exp_();
    auto r = parse_exp_();

    while (r) {
        l = mk<App>(std::move(l), std::move(r));
        r = parse_exp_();
    }

    return l;
}

Ptr<Var> Parser::parse_var() {
    return mk<Var>(eat(Tok::Tag::Id).str());
}

Ptr<Lam> Parser::parse_lam() {
    eat(Tok::Tag::Lam);
    std::string binder = ahead().isa(Tok::Tag::Id) ? lex().str() : std::string("<error>");
    expect(Tok::Tag::Dot);
    return mk<Lam>(binder, parse_exp());
}

}
