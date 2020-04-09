# TranSAT
> A Translated SAT Solver.

Based on the [OKlibrary](https://github.com/OKullmann/oklibrary/) GenericSAT N-Queens solver [(here)](https://github.com/OKullmann/oklibrary/tree/master/Satisfiability/Transformers/Generators/Queens/GenericSAT).

#### Tools

Built with [musl 1.2.0](https://musl.libc.org/) on [gcc 8.3.0](https://gcc.gnu.org/) using `-fplan9-extensions`. [midipix](https://midipix.org/) can be used to convert musl's system calls to Windows Native API calls.

GenericSAT from latest relevant commit: [`abc4193`](https://github.com/OKullmann/oklibrary/commit/abc419334da4e73f44dd1c13cc4d3ae78a534b63).

#### Simple build
```
musl-gcc -fplan9-extensions -static transat.c -o transat
```

#### Optimised build
```
musl-gcc -fplan9-extensions -static \
         -Wall -Wextra -Wno-unused-value -Wno-unused-variable \
         -Ofast -march=native -fwhole-program \
         -DNDEBUG transat.c -o transat
```
