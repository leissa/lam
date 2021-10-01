#include "sym.h"

namespace lam {

SymTab symtab;

Sym SymTab::make(const std::string& str) {
    const auto& [i, _] = strings_.emplace(str);
    return &*i;
}

Sym SymTab::make(const char* str) {
    const auto& [i, _] = strings_.emplace(str);
    return &*i;
}

std::ostream& operator<<(std::ostream& o, Sym sym) {
    return o << *sym;
}

}
