#include "loc.h"

namespace lam {

std::ostream& operator<<(std::ostream& o, Pos pos) {
    return o << pos.row << "." << pos.col;
}

std::ostream& operator<<(std::ostream& o, Loc loc) {
    o << loc.filename << ":" << loc.begin;
    if (loc.begin.row != loc.finis.row) {
        o << "-" << loc.finis;
    } else {
        if (loc.begin.col != loc.finis.col)
            o << "-" << loc.finis.col;
    }

    return o;
}

}
