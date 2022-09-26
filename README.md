# Lam - An Interpreter for the Lambda Calculus

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

## Piping the Output of the C Preprocessor into the Interpreter

Simply use a shell pipe `|` and the special input file `-` to inform `lam` that it should read the input from `stdin`:
```
cpp -P test/fac.lam | ./build/debug/lam -
```
