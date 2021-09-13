# Lam - An Interpreter for the Lambda Calculus

## Building

```
make
```
You can use different configs and specify them via the `CFG` variable like so:
```
make CFG=debug
./build/debug/lam
```

## Piping the output of the C preprocessor into the interpreter

Simply use a shell pipe `|` and the special input file `-` to inform `lam` that it should read the input from `stdin`:
```
cpp -P test/fac.lam | ./build/debug/lam -
```
