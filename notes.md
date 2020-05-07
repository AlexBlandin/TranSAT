./genericsat = ./oklibrary/Satisfiability/Transformers/Generators/Queens/GenericSAT

- propagation
- watched literals


testing uses
- gcc 8.3
- clang 10


 N | Solutions
------------------
14 | 365596
15 | 2279184
16 | 14772512
17 | 95815104
18 | 666090624
19 | 4968057848
20 | 39029188884
21 | 314666222712


Rosetta
 N | Time
-----------------
14 | 0m0.074s
15 | 0m0.407s
16 | 0m2.655s
17 | 0m18.617s
18 | 2m18.946s
19 | 18m7.336s
20 | 147m46.599s
21 | 1235m0.767s


ExpQueens
 N | Time
-----------------
14 | 0m1.734s
15 | 0m9.781s
16 | 1m0.359s
17 | 6m27.906s
18 | 45m49.609s


qcount d
 N | Time
-----------------
14 | 0m0.188s
15 | 0m1.250s
16 | 0m7.875s
17 | 0m59.125s
18 | 7m1.172s


TranSAT v0.1
 N | FirstRow
-----------------
 8 | 0m0.188s
 9 | 0m1.875s
10 | 0m19.188s
11 | 3m28.703s
12 | 42m53.125s


TranSAT v0.3
 N | FirstRow
-----------------
 8 | 0m0.016s
 9 | 0m0.094s
10 | 0m0.703s
11 | 0m7.047s
12 | 1m16.203s
13 | 13m39.484s


TranSAT v0.3.5 (v0.3 but using bitsets and taking out the old state, shrinking mem. footprint, leaving the old state in as ballast gives v0.3 timings, proving size matters for prefetching)
 N | FirstRow
-----------------
 8 | 0m0.016s
 9 | 0m0.078s
10 | 0m0.688s
11 | 0m6.234s
12 | 1m2.828s
13 | 10m55.922s


TranSAT v0.3.5 (Square Enumeration)
 N | SquareEnum
-----------------
 8 | 0m0.016s
 9 | 0m0.063s
10 | 0m0.594s
11 | 0m5.484s
12 | 0m54.453s
13 | 9m19.594s


TranSAT v0.4
 N | FirstRow
-----------------
 8 | 0m0.016s
 9 | 0m0.063s
10 | 0m0.641s
11 | 0m5.922s
12 | 0m57.219s
13 | 9m33.203s

 N | SquareEnum
-----------------
 8 | 0m0.016s
 9 | 0m0.063s
10 | 0m0.563s
11 | 0m5.031s
12 | 0m49.469s
13 | 8m24.438s


TranSAT v0.4.5
 N | FirstRow   | SquareEnum | Taw
-------------------------------------------
 9 | 0m0.031s   | 0m0.031s   | 0m0.063s
10 | 0m0.313s   | 0m0.297s   | 0m0.516s
11 | 0m2.922s   | 0m2.563s   | 0m4.406s
12 | 0m26.266s  | 0m23.484s  | 0m38.844s
13 | 4m55.516s  | 4m6.594s   | 6m23.750s
14 | 52m29.656s | 44m25.094s | 65m55.891s


TranSAT v0.4.5 + 32B Ballast / board
 N | FirstRow
-----------------
 9 | 0m0.031s
10 | 0m0.344s
11 | 0m2.953s
12 | 0m27.500s
13 | 5m5.031s

 N | SquareEnum
-----------------
 9 | 0m0.031s
10 | 0m0.297s
11 | 0m2.609s
12 | 0m24.391s
13 | 4m22.313s

 N | Taw
-----------------
 9 | 0m0.063s
10 | 0m0.531s
11 | 0m4.547s
12 | 0m40.078s
13 | 6m27.000s


TranSAT v0.4.5 + 64B Ballast / board
 N | FirstRow
-----------------
 9 | 0m0.031s
10 | 0m0.344s
11 | 0m2.984s
12 | 0m28.078s
13 | 5m7.391s

 N | SquareEnum
-----------------
 9 | 0m0.047s
10 | 0m0.313s
11 | 0m2.625s
12 | 0m25.094s
13 | 4m18.781s

 N | Taw
-----------------
 9 | 0m0.063s
10 | 0m0.531s
11 | 0m4.656s
12 | 0m40.734s
13 | 6m35.500s


TranSAT v0.4.5 + 128B Ballast / board
 N | FirstRow
-----------------
 9 | 0m0.047s
10 | 0m0.359s
11 | 0m3.469s
12 | 0m30.391s
13 | 5m49.969s

 N | SquareEnum
-----------------
 9 | 0m0.047s
10 | 0m0.344s
11 | 0m3.094s
12 | 0m27.734s
13 | 5m1.359s

 N | Taw
-----------------
 9 | 0m0.078s
10 | 0m0.594s
11 | 0m5.031s
12 | 0m44.328s
13 | 7m39.734s


TranSAT v0.4.5 + 256B Ballast / board
 N | FirstRow
-----------------
 9 | 0m0.047s
10 | 0m0.391s
11 | 0m3.453s
12 | 0m30.516s
13 | 5m53.125s

 N | SquareEnum
-----------------
 9 | 0m0.063s
10 | 0m0.344s
11 | 0m3.141s
12 | 0m28.453s
13 | 5m7.172s

 N | Taw
-----------------
 9 | 0m0.078s
10 | 0m0.594s
11 | 0m5.094s
12 | 0m44.906s
13 | 7m46.281s



TranSAT v0.4.5 + 512B Ballast / board
 N | FirstRow
-----------------
 9 | 0m0.063s
10 | 0m0.391s
11 | 0m3.953s
12 | 0m32.141s
13 | 6m45.281s

 N | SquareEnum
-----------------
 9 | 0m0.063s
10 | 0m0.375s
11 | 0m3.578s
12 | 0m29.516s
13 | 5m57.094s

 N | Taw
-----------------
 9 | 0m0.094s
10 | 0m0.656s
11 | 0m5.578s
12 | 0m46.563s
13 | 8m44.766s


TranSAT v0.4.5 + 1KiB Ballast / board

 N | FirstRow
-----------------
 9 | 0m0.047s
10 | 0m0.422s
11 | 0m4.156s
12 | 0m34.578s
13 | 7m6.906s
14 | 67m49.984s

 N | SquareEnum
-----------------
 9 | 0m0.047s
10 | 0m0.391s
11 | 0m3.797s
12 | 0m32.047s
13 | 6m22.359s
14 | 58m53.078s

 N | Taw
-----------------
 9 | 0m0.078s
10 | 0m0.641s
11 | 0m5.859s
12 | 0m48.594s
13 | 9m5.797s
14 | 83m22.328s


TranSAT v0.5
 N | Time
-----------------
11 | 
12 | 
13 | 
14 | 
15 | 
16 | 

TranSAT v1.0
 N | Time
-----------------
11 | 
12 | 
13 | 
14 | 
15 | 
16 | 
17 | 
18 | 
19 | 
20 | 
21 | 



[SIMD](https://software.intel.com/sites/landingpage/IntrinsicsGuide/)

target <= SSE 4.2, may be able to add AVX & AVX 2 for desktop & servers

    #include <x86intrin.h>

or, for slightly more portability (aka MSVC(/ICC?) support) but a subset,

    #include <immintrin.h>

or, conditionally replace with

    #include <intrin.h>
