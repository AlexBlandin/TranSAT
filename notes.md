./genericsat = ./oklibrary/Satisfiability/Transformers/Generators/Queens/GenericSAT

- propagation
- watched literals


testing can use
- gcc 8.3
- gcc 9.3
- gcc trunk
- clang 7
- clang 10



Rosetta
 N | Solution    | Time
----------------------
14 | 365596      | 0m0.074s
15 | 2279184     | 0m0.407s
16 | 14772512    | 0m2.655s
17 | 95815104    | 0m18.617s
18 | 666090624   | 2m18.946s
19 | 4968057848  | 18m7.336s
20 | 39029188884 | 147m46.599s



#### probably can't do in retrospect

[SIMD](https://software.intel.com/sites/landingpage/IntrinsicsGuide/)

target <= SSE 4.2, may be able to add AVX & AVX 2 for desktop & servers

    #include <x86intrin.h>

or, for slightly more portability (aka MSVC(/ICC?) support) but a subset,

    #include <immintrin.h>

or, conditionally replace with

    #include <intrin.h>
