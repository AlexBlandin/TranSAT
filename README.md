# TranSAT
> A Translated SAT Solver for N-Queens.

Based on the [GenericSAT](https://github.com/OKullmann/oklibrary/tree/master/Satisfiability/Transformers/Generators/Queens/GenericSAT) solver for N-Queens via Active Clause Sets (ACLS) in the [OKlibrary](https://github.com/OKullmann/oklibrary/).

#### Tools

Built with [musl 1.2.0](https://musl.libc.org/) on [gcc 8.3.0](https://gcc.gnu.org/). [midipix](https://midipix.org/) can be used to convert musl's system calls to Windows' [Native API](https://en.wikipedia.org/wiki/Native_API) calls.

Can use [clang 10.0.0](https://clang.llvm.org/) for a more portable Unix/Windows compilation experience.

Reference GenericSAT from commit [`abc4193`](https://github.com/OKullmann/oklibrary/commit/abc419334da4e73f44dd1c13cc4d3ae78a534b63).

#### Simple build
```bash
$ musl-gcc -static transat.c -o transat
```

#### Build Script
```bash
$ ./build.sh transat <N>
```

#### Simple Timings
```bash
$ ./timeit.sh
```

#### Ballast Timings
```bash
$ ./ballast.sh <N>
```

#### Profile
```bash
$ gcc --coverage -DN=11 -o transat transat.c && ./transat && gcov transat.c -m
```

#### RemedyBG
```cmd
> clang-cl -fuse-ld=lld -Z7 -MTd transat.c -o transat.exe && remedybg dbg.rdbg
```

#### TODO:

- Fix propagation regression (and thus performance regression)
- Add full symmetry breaking (~8x)
- New heuristics

<!-- COPYRIGHT Alex Blandin (c) 2020 -->
<!-- See LICENSE -->
