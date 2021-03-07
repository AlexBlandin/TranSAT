# TranSAT
> A Translated SAT Solver for N-Queens.

Based on the [GenericSAT](https://github.com/OKullmann/oklibrary/tree/master/Satisfiability/Transformers/Generators/Queens/GenericSAT) solver for N-Queens via Active Clause Sets (ACLS) in the [OKlibrary](https://github.com/OKullmann/oklibrary/).

### Tools

Originally built with [musl 1.2.0](https://musl.libc.org/) on [gcc 8.3.0](https://gcc.gnu.org/). [midipix](https://midipix.org/) can be used to convert musl's system calls to Windows' [Native API](https://en.wikipedia.org/wiki/Native_API) calls.

The most recent commits are a test of [Cosmopolitan libc](https://justine.lol/cosmopolitan/index.html) using GCC (add `-Icosmo --DNDEBUG -DN=13` to standard Cosmopolitan `gcc` arguments, direct includes to cosmo folder).
In theory, not only does this produce a portable executable (tested, it works) but it might also be faster than musl and glibc for static builds (not yet tested). See [Cosmopolitan libc](https://justine.lol/cosmopolitan/index.html).

Can use [clang 10.0.0](https://clang.llvm.org/) or [zig 0.7.1+](https://ziglang.org) ([zig cc](https://andrewkelley.me/post/zig-cc-powerful-drop-in-replacement-gcc-clang.html)) for a more portable Unix/Windows compilation experience without Cosmopolitan libc.

Reference GenericSAT from commit [`abc4193`](https://github.com/OKullmann/oklibrary/commit/abc419334da4e73f44dd1c13cc4d3ae78a534b63).

### Simple build
```bash
$ musl-gcc -static transat.c -o transat
```

### Cosmopolitan Build
```bash
$ gcc -g -Os -static -fno-pie -no-pie -mno-red-zone -nostdlib -nostdinc \
  -fno-omit-frame-pointer -pg -mnop-mcount -Wl,--build-id=none -Icosmo \
  --DNDEBUG -DN=13 -o transat.com.dbg transat.c -Wl,--gc-sections -fuse-ld=bfd \
  -Wl,-T,cosmo/ape.lds  cosmo/cosmopolitan.h cosmo/crt.o cosmo/ape.o cosmo/cosmopolitan.a
$ objcopy -SO binary transat.com.dbg transat.com
```

### Build Script
```bash
$ ./build.sh transat <N>
```

Or, with Cosmopolitan Libc

```bash
$ ./build-cosmo.sh transat <N> 
```

### Simple Timings
```bash
$ ./timeit.sh
```

### Ballast Timings
```bash
$ ./ballast.sh <N>
```

### Profile
```bash
$ gcc --coverage -DN=11 -o transat transat.c && ./transat && gcov transat.c -m
```

### RemedyBG
```cmd
> clang-cl -fuse-ld=lld -Z7 -MTd transat.c -o transat.exe && remedybg dbg.rdbg
```

#### TODO:

- Fix propagation regression (and thus performance regression)
- Add full symmetry breaking (~8x)
- New heuristics

<!-- COPYRIGHT Alex Blandin (c) 2021 -->
<!-- See LICENSE -->
