./genericsat = ./oklibrary/Satisfiability/Transformers/Generators/Queens/GenericSAT

- propagation
- watched literals


testing can use
- gcc 8.3
- gcc 9.3
- gcc trunk
- clang 7
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
 N | FIRSTROW
-----------------
 8 | 0m0.188s
 9 | 0m1.875s
10 | 0m19.188s
11 | 3m28.703s
12 | 42m53.125s

Transat v0.3
 N | FIRSTROW
-----------------
 8 | 0m0.016s
 9 | 0m0.094s
10 | 0m0.703s
11 | 0m7.047s
12 | 1m16.203s
13 | 13m39.484s

Transat v0.3.5 (v0.3 but using bitsets and taking out the old state, shrinking mem. footprint, leaving the old state in as ballast gives v0.3 timings, proving size matters for prefetching)
 N | FIRSTROW
-----------------
 8 | 0m0.016s
 9 | 0m0.078s
10 | 0m0.688s
11 | 0m6.234s
12 | 1m2.828s
13 | 10m55.922s

Transat v0.3.5 (Square Enumeration)
 N | SQUAREENUM
-----------------
 8 | 0m0.016s
 9 | 0m0.063s
10 | 0m0.594s
11 | 0m5.484s
12 | 0m54.453s
13 | 9m19.594s

Transat v0.4
 N | FIRSTROW
-----------------
 8 | 0m0.016s
 9 | 0m0.063s
10 | 0m0.641s
11 | 0m5.922s
12 | 0m57.219s
13 | 9m33.203s

 N | SQUAREENUM
-----------------
 8 | 0m0.016s
 9 | 0m0.063s
10 | 0m0.563s
11 | 0m5.031s
12 | 0m49.469s
13 | 8m24.438s

Transat v0.4.5
 N | FIRSTROW
-----------------
 9 | 0m0.031s
10 | 0m0.313s
11 | 0m2.953s
12 | 0m26.718s
13 | 4m59.828s
14 | 52m33.047s

 N | SQUAREENUM
-----------------
 9 | 0m0.031s
10 | 0m0.379s
11 | 0m2.594s
12 | 0m23.984s
13 | 4m39.500s
14 | 49m41.328s

 N | TAW
-----------------
 9 | 0m0.063s
10 | 0m0.516s
11 | 0m4.406s
12 | 0m38.844s
13 | 6m23.750s
14 | 65m55.891s

ANTITAW is just TOO SLOW (bc. it has to redo everything and go from the least changes each time so it does the most work for least gain)




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


      If I expand Rank to store open, even for just the diags (adiags are easy to calc) that's expensive
      I would need to take the queens out again to drop from 124kB to 99kB...

>>> print(" N |  Size   | Maybe\n---------------------"); [print(f"{N} | {ns(x:=N+8+2+(N*N+7)//8+N*N*(3+1*(N*N+7)//8+(N+N+2*(2*N+2*N-1-1))))} | {ns(x/N)}") for N in range(16,25)][0]
 N |  Size   | Maybe
---------------------
16 | 49.0 kB | 3.1 kB
17 | 59.6 kB | 3.5 kB
18 | 71.3 kB | 4.0 kB
19 | 84.9 kB | 4.5 kB
20 | 99.7 kB | 5.0 kB
21 | 117.0 kB | 5.6 kB
22 | 135.6 kB | 6.2 kB
23 | 156.7 kB | 6.8 kB
24 | 179.2 kB | 7.5 kB
>>> print(" N |  Size   | Maybe\n---------------------"); [print(f"{N} | {ns(x:=N+8+2+(N*N+7)//8+N*N*(3+1*(N*N+7)//8+(N+N+2*(2*N-1)+2*N-1)))} | {ns(x/N)}") for N in range(16,25)][0]
 N |  Size   | Maybe
---------------------
16 | 41.0 kB | 2.6 kB
17 | 50.1 kB | 2.9 kB
18 | 60.0 kB | 3.3 kB
19 | 71.6 kB | 3.8 kB
20 | 84.1 kB | 4.2 kB
21 | 98.9 kB | 4.7 kB
22 | 114.8 kB | 5.2 kB
23 | 132.9 kB | 5.8 kB
24 | 152.2 kB | 6.3 kB
>>> print(" N |  Size   | Maybe\n---------------------"); [print(f"{N} | {ns(x:=0*N+8+2+(N*N+7)//8+N*N*(3+2*(N*N+7)//8+(N+N+2*(2*N-1)+2*N-1)))} | {ns(x/N)}") for N in range(16,25)][0]
 N |  Size   | Maybe
---------------------
16 | 49.5 kB | 3.1 kB
17 | 60.7 kB | 3.6 kB
18 | 73.3 kB | 4.1 kB
19 | 88.1 kB | 4.6 kB
20 | 104.5 kB | 5.2 kB
21 | 123.5 kB | 5.9 kB
22 | 144.3 kB | 6.6 kB
23 | 168.3 kB | 7.3 kB
24 | 194.2 kB | 8.1 kB




#### probably can't do in retrospect

[SIMD](https://software.intel.com/sites/landingpage/IntrinsicsGuide/)

target <= SSE 4.2, may be able to add AVX & AVX 2 for desktop & servers

    #include <x86intrin.h>

or, for slightly more portability (aka MSVC(/ICC?) support) but a subset,

    #include <immintrin.h>

or, conditionally replace with

    #include <intrin.h>
