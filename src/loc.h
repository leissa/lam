#ifndef LAM_LOC_H
#define LAM_LOC_H

#include <ostream>

namespace lam {

struct Pos {
    int row = -1;
    int col = -1;
};

struct Loc {
    const char* file = nullptr;
    Pos begin;
    Pos finis;
};

std::ostream& operator<<(std::ostream&, const Pos&);
std::ostream& operator<<(std::ostream&, const Loc&);


}

#endif
