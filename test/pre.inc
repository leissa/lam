#define succ (lam n. lam f. lam x. f (n f x))

#define _0 (lam f. lam x. x)
#define _1 (succ _0)
#define _2 (succ _1)
#define _3 (succ _2)
#define _4 (succ _3)
#define _5 (succ _4)
#define _6 (succ _5)
#define _7 (succ _6)
#define _8 (succ _7)
#define _9 (succ _8)

#define pred (lam n. lam f. lam x. n (lam g. lam h. h (g f)) (lam u. x) (lam u. u))
#define add (lam m. lam n. n succ m)
#define mul (lam m. lam n. lam f. lam x. m (n f) x)

#define T (lam x. lam y. x)
#define F (lam x. lam y. y)
#define iszero (lam n. lam x. lam y. n (lam z. y) x)
#define if (lam x. x)

#define Y (lam h. ((lam x. (h (lam a. ((x x) a)))) (lam x. (h (lam a. ((x x) a))))))
#define Theta ((lam x. lam y. y (x x y)) (lam x. lam y. y (x x y)))
