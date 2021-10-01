#ifndef LAM_SYM_H
#define LAM_SYM_H

#include <string>
#include <unordered_set>

namespace lam {

using Sym = const std::string*;

std::ostream& operator<<(std::ostream&, Sym);

class SymTab {
public:
    Sym make(const std::string&);
    Sym make(const char*);

private:
    std::unordered_set<std::string> strings_;
};

struct SymHash {
    size_t operator()(Sym sym) const { return std::hash<std::string>()(*sym); }
};

// I don't recommend global variables in production code but for our toy project it's fine.
extern SymTab symtab;

}

#endif
