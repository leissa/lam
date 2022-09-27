# Lam - An Interpreter for the Lambda Calculus

[![Makefile CI](https://github.com/leissa/lam/actions/workflows/makefile.yml/badge.svg)](https://github.com/leissa/lam/actions/workflows/makefile.yml)

## Building

```sh
make
```
You can use different configs and specify them via the `CFG` variable like so:
```sh
make CFG=debug
./build/debug/lam
```
You can enable a more verbose output of the build process like so:
```sh
make CFG=debug Q=
```

## Grammar

```ebnf
e = ID                  (* variable *)
  | e e                 (* application *)
  | lam ID '.' e        (* abstraction *)
  | '(' e ')'           (* paranthesized expression *)
  | 'let' ID e ';' e    (* let expression *)
  ;
```
where
* `ID` = [`a`-`zA`-`Z``_`][`a`-`zA`-`Z0`-`9``_`]*

In addition you can use [C/C++-style comments](https://en.cppreference.com/w/c/comment):
```c++
lam x. /* C-style comment */ x // C++-style end-of-line comment
```

**Note:** A *let expression* `let ID e_a; e_b` is just syntactic sugar for `(lam ID. e_b) e_a`.

### Precedence

Ambiguities in the expression productions are resolved according to the following rules:
* Applications associate to the left: `e_1 e_2 e_3` = `(e_1 e_2) e_3`
* Applications bind stronger than abstractions: `lam x. e_1 e_2` = `lam x. (e_1 e_2)`
