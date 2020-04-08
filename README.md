# TranSAT
A Translated SAT Solver

Based on the (OKlibrary)[https://github.com/OKullmann/oklibrary/] GenericSAT N-Queens solver (here)[https://github.com/OKullmann/oklibrary/tree/master/Satisfiability/Transformers/Generators/Queens/GenericSAT].

Built with (musl 1.2.0)[https://musl.libc.org/]. GenericSAT at head (latest relevant commit: (`abc4193`)[https://github.com/OKullmann/oklibrary/commit/abc419334da4e73f44dd1c13cc4d3ae78a534b63], commit at time of `git pull`ing: (`b69d68e`)[https://github.com/OKullmann/oklibrary/commit/b69d68ee25edab5c4b3f0fc8438886a03a2e0f26])

#### Simple build
```
> musl-gcc -static transat.c -o transat
```

#### Optimised build
```
> musl-gcc -static -Wall -Wextra -Wno-unused-value -Ofast -march=native -fwhole-program -DNDEBUG transat.c -o transat
```
