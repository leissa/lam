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

Ptr<Exp> Parser::parse_prg() {
    auto exp = parse_exp("program");
    expect(Tok::Tag::EoF, "program");
    return exp;
}

Ptr<Exp> Parser::parse_exp(const char* ctxt) {
    auto track = tracker();

    if (auto l = parse_exp_()) {
        auto r = parse_exp_();

        while (r) {
            l = mk<App>(track, std::move(l), std::move(r));
            r = parse_exp_();
        }

        return l;
    }

    err("expression", ctxt);
    return mk<Err>(prev_);
}

Ptr<Exp> Parser::parse_exp_() {
    switch (ahead().tag()) {
        case Tok::Tag::Id:  return parse_var();
        case Tok::Tag::Lam: return parse_lam();
        case Tok::Tag::Let: return parse_let();
        case Tok::Tag::Paren_L: {
            lex();
            auto res = parse_exp("parenthesized expression");
            expect(Tok::Tag::Paren_R, "parenthesized expression");
            return res;
        }
        default:            return nullptr;
    }
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
    return mk<Lam>(track, binder, parse_exp("body of a lambda expression"));
}

Ptr<App> Parser::parse_let() {
    auto track = tracker();
    eat(Tok::Tag::Let);
    std::string binder = ahead().isa(Tok::Tag::Id) ? lex().str() : std::string("<error>");
    expect(Tok::Tag::Assign, "let expression");
    auto arg = parse_exp("let expression");
    expect(Tok::Tag::In, "let expression");
    auto body = parse_exp("body of a let expression");
    auto lam = mk<Lam>(track, binder, std::move(body));
    return mk<App>(track, std::move(lam), std::move(arg), true);
}

}
