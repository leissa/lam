#include "tok.h"

namespace lam {

const char* Tok::tag2str(Tok::Tag tag) {
    switch (tag) {
#define CODE(t, str) case Tok::Tag::t: return str;
        LAM_KEYWORDS(CODE)
        LAM_PUNCTUATORS(CODE)
        LAM_MISC(CODE)
#undef CODE
    }

    return nullptr; // shutup warning
}

std::ostream& operator<<(std::ostream& o, const Tok& tok) {
    if (tok.isa(Tok::Tag::M_id)) return o << *tok.sym();
    return o << Tok::tag2str(tok.tag());
}

}
