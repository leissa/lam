#include "loc.h"

namespace lam {

std::ostream& operator<<(std::ostream& o, const Pos& pos) {
    return o << pos.row << ":" << pos.col;
}

std::ostream& operator<<(std::ostream& o, const Loc& loc) {
    o << loc.file << ":" << loc.begin;
    if (loc.begin.row != loc.finis.row) {
        o << "-" << loc.finis;
    } else {
        if (loc.begin.col != loc.finis.col)
            o << "-" << loc.finis.col;
    }

    return o;
}

}
