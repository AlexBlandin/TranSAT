#ifndef TRANSAT_H_IMPLEMENTED
#define TRANSAT_H_IMPLEMENTED

// 24 is the max
#define N 24

// n MUST BE A CONSTANT
#define queen(n) if (n < N) queens.q##n

// Aligned to 8byte boundaries
typedef union _Queens {
  struct {
    u8 q0, q1, q2, q3, q4, q5, q6, q7;
    u8 q8, q9, q10, q11, q12, q13, q14, q15;
    u8 q16, q17, q18, q19, q20, q21, q22, q23;
  };
  u8 q[N];
} Queens;

typedef union _Rank {
  struct {
  u8 placed: 1; // AMO means only one so this can be a bit
  u8 forbidden: 5; // this can be 0..N (for unsats.) so needs 5 bits (ceil(lg(24)))
  }; // `open` is easy to calculate, including would mean using u16 (the difference is 70kB at N=20 vs 120kB)
  u8 _r; // I personally always have a union over bitfields in case I need to address the entire thing
} Rank;

/* DATA */
static Queens queens;
static u32 queens_mask[N*N] = {}; // which queens are placed (bitmask)
static u8 board[N*N][N*N/8] = {}; // open/forbidden ALCS boards
static Rank ranks[N*N][6*N - 2] = {}; // placed/forbidden ranks

/*
> python3
>>> from humanize import naturalsize as ns
>>> print(" N |  Size\n------------"); [print(f"{N} | {ns(N + (N*N*4) + (N**4)//8 + N*N*(6*N - 2))}") for N in range(16,25)][0]
 N |  Size
------------
16 | 33.3 kB
17 | 40.5 kB
18 | 48.8 kB
19 | 58.2 kB
20 | 68.8 kB
21 | 80.8 kB
22 | 94.2 kB
23 | 109.1 kB
24 | 125.6 kB
>>>
*/

static u64 solutions[] = {1, 1, 0, 0, 2, 10, 4, 40, 92, 352,
                          724, 2680, 14200, 73712, 365596,
                          2279184, 14772512, 95815104, 666090624,
                          4968057848, 39029188884, 314666222712,
                          2691008701644, 24233937684440,
                          227514171973736, 2207893435808352,
                          22317699616364044, 234907967154122528};

void init() {
  seed_rng();
  assert(N < 24);
  assert(sizeof(Rank) == 1); // Should just be a byte
}


#endif // TRANSAT_H_IMPLEMENTED
