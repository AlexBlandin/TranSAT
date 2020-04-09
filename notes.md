./genericsat = ./oklibrary/Satisfiability/Transformers/Generators/Queens/GenericSAT

- watched literals





#### probably can't do in retrospect

[SIMD](https://software.intel.com/sites/landingpage/IntrinsicsGuide/)

target <= SSE 4.2, may be able to add AVX & AVX 2 for desktop & servers

    #include <x86intrin.h>

or, for slightly more portability (aka MSVC(/ICC?) support) but a subset,

    #include <immintrin.h>

or, conditionally replace with

    #include <intrin.h>
