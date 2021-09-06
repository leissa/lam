#include "ast.h"

using namespace lam;

int main() {
    auto p = mk<Var>("x");
    auto x = p->eval();
    //x->dump();
    // (lam x. x) z
    auto p1 = mk<App>(mk<Lam>("x", mk<Var>("x")), mk<Var>("z"));
    p1->dump();

    auto p2 = p1->clone();
    p1->dump();
    p1.reset();
    p2->dump();

    // λf. λx. x
    auto n0 = mk<Lam>("f", mk<Lam>("x", mk<Var>("x")));
    // λf. λx. f x
    auto n1 = mk<Lam>("f", mk<Lam>("x", mk<App>(mk<Var>("f"), mk<Var>("x"))));
    // λf. λx. f (f x)
    auto n2 = mk<Lam>("f", mk<Lam>("x", mk<App>(mk<Var>("f"), mk<App>(mk<Var>("f"), mk<Var>("x")))));

    n0->dump();
    n1->dump();
    n2->dump();

    // λn. λf. λx. f (n f x)
    auto succ = mk<Lam>("n", mk<Lam>("f", mk<Lam>("x", mk<App>(mk<Var>("f"), mk<App>(mk<App>(mk<Var>("n"), mk<Var>("f")), mk<Var>("x"))))));
    succ->dump();

    mk<App>(succ->clone(), n1->clone())->eval()->dump();
    n2->dump();

    //auto p3 = p2->eval();
    //p3->dump();

    return 0;
}
