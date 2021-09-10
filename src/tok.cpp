#include "tok.h"

namespace lam {

const char* Tok::tag2str(Tok::Tag tag) {
    switch (tag) {
        case Tok::Tag::EoF:     return "<eof>";
        case Tok::Tag::Id:      return "<identifier>";
        case Tok::Tag::Lam:     return "lam";
        case Tok::Tag::Dot:     return ".";
        case Tok::Tag::Paren_L: return "(";
        case Tok::Tag::Paren_R: return ")";
        default: assert(false && "unreachable");
    }
}

std::ostream& operator<<(std::ostream& o, const Tok& tok) {
    if (tok.isa(Tok::Tag::Id)) return o << tok.str();
    return o << Tok::tag2str(tok.tag());
}

}
