#ifndef LAM_PARSER_H
#define LAM_PARSER_H

#include "ast.h"
#include "lexer.h"

namespace lam {

class Parser {
public:
    Parser(const char* file, std::istream& stream);

    Ptr<Exp> parse_prg();

private:
    Ptr<Exp> parse_exp(const char* ctxt);
    Ptr<Exp> parse_exp_();
    Ptr<Var> parse_var();
    Ptr<Lam> parse_lam();

    class Tracker {
    public:
        Tracker(Parser& parser, const Pos& pos)
            : parser_(parser)
            , pos_(pos)
        {}

        operator Loc() const { return {parser_.prev_.file, pos_, parser_.prev_.finis}; }

    private:
        Parser& parser_;
        Pos pos_;
    };

    Tracker tracker() { return Tracker(*this, ahead().loc().begin); }
    Tok lex();
    Tok ahead() const { return ahead_; }
    bool accept(Tok::Tag);
    bool expect(Tok::Tag, const char*);
    Tok eat([[maybe_unused]] Tok::Tag tag) { assert(tag == ahead().tag() && "internal parser error"); return lex(); }
    void err(const std::string& what, const char* ctxt) { err(what, ahead(), ctxt); }
    void err(const std::string& what, const Tok& tok, const char* ctxt);

    Lexer lexer_;
    Loc prev_;
    Tok ahead_;
};

}

#endif
