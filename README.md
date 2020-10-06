# anthem

> Translate answer set programs to first-order theorem prover language

## Overview

`anthem` translates ASP programs (in the input language of [`clingo`](https://github.com/potassco/clingo)) to the first-order logic. 
The output can be formatted in either a human-readable format or the TPTP format for usage with automated theorem provers such as [`vampire`](https://github.com/vprover/vampire) or [`cvc4`](https://github.com/CVC4/CVC4).

## Usage

```bash
$ anthem [options] file1 file2
```

By default, `anthem` uses the translation mode `here-and-there`.
Given two ASP programs as input, `anthem` produces a formula asserting the strong equivalence of the two programs.

By using the option `--output-format tptp` theorem provers such as [`vampire`](https://github.com/vprover/vampire) or [`cvc4`](https://github.com/CVC4/CVC4) can then be used on the output of `anthem` to verify the strong equivalence of the two input programs.

Furthermore, `anthem` can use the mode `completion` to perform the Clark’s completion on the translated formulas.

## Building

`anthem` requires [CMake](https://cmake.org/) for building.
After installing the dependencies, `anthem` is built with a C++17 compiler (GCC ≥ 7.3 or clang ≥ 5.0).

```bash
$ git clone https://github.com/janheuer/anthem.git
$ cd anthem
$ git submodule update --init --recursive
$ mkdir -p build/release
$ cd build/release
$ cmake ../.. -DCMAKE_BUILD_TYPE=Release
$ make
```

## Contributors

* [Patrick Lühne](https://www.luehne.de)
* Jan Heuer
