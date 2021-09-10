#include "parser.h"

#include <fstream>
#include <iostream>

namespace lam {

Parser::Parser(const char* file, std::istream& stream)
    : lexer_(file, stream)
    , prev_(lexer_.loc())
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

bool Parser::expect(Tok::Tag tag, const char* ctxt) {
    if (ahead().tag() == tag) {
        lex();
        return true;
    }

    err(std::string("'") + Tok::tag2str(tag) + std::string("'"), ctxt);
    return false;
}

void Parser::err(const std::string& what, const Tok& tok, const char* ctxt) {
    std::cerr << tok.loc() << ": expected " << what << ", got '" << tok << "' while parsing " << ctxt << std::endl;
}

Ptr<Exp> Parser::parse_exp_() {
    switch (ahead().tag()) {
        case Tok::Tag::Id:  return parse_var();
        case Tok::Tag::Lam: return parse_lam();
        case Tok::Tag::Paren_L: {
            lex();
            auto res = parse_exp();
            expect(Tok::Tag::Paren_R, "primary expression");
            return res;
        }
        default:
            lex();
            return mk<Err>(prev_);
    }
}

Ptr<Exp> Parser::parse_exp() {
    auto track = tracker();
    auto l = parse_exp_();
    auto r = parse_exp_();

    while (!dynamic_cast<Err*>(r.get())) {
        l = mk<App>(track, std::move(l), std::move(r));
        r = parse_exp_();
    }

    return l;
}

Ptr<Var> Parser::parse_var() {
    auto tok = eat(Tok::Tag::Id);
    return mk<Var>(tok.loc(), tok.str());
}

Ptr<Lam> Parser::parse_lam() {
    auto track = tracker();
    eat(Tok::Tag::Lam);
    std::string binder = ahead().isa(Tok::Tag::Id) ? lex().str() : std::string("<error>");
    expect(Tok::Tag::Dot, "lambda expression");
    return mk<Lam>(track, binder, parse_exp());
}

}
