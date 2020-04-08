# TranSAT
> A Translated SAT Solver.

Based on the [OKlibrary](https://github.com/OKullmann/oklibrary/) GenericSAT N-Queens solver [(here)](https://github.com/OKullmann/oklibrary/tree/master/Satisfiability/Transformers/Generators/Queens/GenericSAT).

#### Sources

Built with [musl 1.2.0](https://musl.libc.org/).

GenericSAT from latest relevant commit: [`abc4193`](https://github.com/OKullmann/oklibrary/commit/abc419334da4e73f44dd1c13cc4d3ae78a534b63).

#### Simple build
```
) musl-gcc -static transat.c -o transat
```

#### Optimised build
```
) musl-gcc -static -Wall -Wextra -Wno-unused-value -Ofast -march=native -fwhole-program -DNDEBUG transat.c -o transat
```
