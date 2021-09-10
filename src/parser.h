#ifndef LAM_PARSER_H
#define LAM_PARSER_H

#include "ast.h"
#include "lexer.h"

namespace lam {

class Parser {
public:
    Parser(const char* filename, std::istream& stream);

    Ptr<Exp> parse_exp();
    Ptr<Exp> parse_exp_();
    Ptr<Var> parse_var();
    Ptr<Lam> parse_lam();

private:
    Tok lex();
    const Tok& ahead() const { return ahead_; }
    Tok eat(Tok::Tag tag) { assert(tag == ahead().tag() && "internal parser error"); return lex(); }
    bool accept(Tok::Tag tok);
    bool expect(Tok::Tag tok);
    void err();

    Lexer lexer_;
    Tok ahead_;
};

}

#endif
