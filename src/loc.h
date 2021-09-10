#ifndef LAM_LOC_H
#define LAM_LOC_H

#include <ostream>

namespace lam {

struct Pos {
    int row = -1;
    int col = -1;
};

struct Loc {
    Pos begin;
    Pos finis;
    const char* filename = nullptr;
};

std::ostream& operator<<(std::ostream&, Pos);
std::ostream& operator<<(std::ostream&, Loc);

}

#endif
