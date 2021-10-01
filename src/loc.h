#ifndef LAM_LOC_H
#define LAM_LOC_H

#include <ostream>

namespace lam {

// I don't recommend global variables in production code but for our toy project it's fine.
extern int num_errors;

struct Pos {
    Pos() = default;
    Pos(int row, int col)
        : row(row)
        , col(col)
    {}

    int row = -1;
    int col = -1;
};

struct Loc {
    Loc() = default;
    Loc(const char* file, Pos begin, Pos finis)
        : file(file)
        , begin(begin)
        , finis(finis)
    {}
    Loc(const char* file, Pos pos)
        : Loc(file, pos, pos)
    {}

    const char* file = nullptr;
    Pos begin;
    Pos finis;

    std::ostream& err() const;
};

std::ostream& operator<<(std::ostream&, const Pos&);
std::ostream& operator<<(std::ostream&, const Loc&);

}

#endif
