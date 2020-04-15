#ifndef TRANSAT_H_IMPLEMENTED
#define TRANSAT_H_IMPLEMENTED

// 24 is the max
#define N 24

// n MUST BE A CONSTANT
#define queen(n) if (n < N) queens.q##n
#define sl boards[board].slot
#define bd boards[board]
#define sl_dia (sl.row + sl.col)
#define sl_adg (N - sl.col + sl.row - 1)

typedef union _Rank {
  struct {
  u8 placed: 1; // AMO means only one so this can be a bit
  u8 forbidden: 5; // this can be 0..N (for unsats.) so needs 5 bits (ceil(lg(24)))
  }; // `open` is easy to calculate, including would mean using u16 (the difference is 70kB at N=20 vs 120kB)
  u8 rank; // I personally always have a union over bitfields in case I need to address the entire thing
} Rank;

typedef struct _Ranks {
  Rank rows[N]; // how many are placed/forbidden
  Rank cols[N]; // (open = N - closed, closed = placed + forbidden))...
  Rank dias[2*N-1];
  Rank adia[2*N-1];
} Ranks;

typedef struct _Board {
  u8 forbid[bits(N*N)]; // 0=open, 1=forbid
  u8 placed[bits(N*N)]; // 0=no queen, 1=queen
  Ranks ranks;
  Slot slot; // where we changed (either forbid or placed)
} Board;

typedef struct _Slot {
  i8 row; // better to have separated row/col & multiply it than keep dividing
  i8 col;
} Slot;

/* DATA */
static u64 nq = 0; // solutions
static u16 board = 0; // current board
static u8 progress[bits(N*N)] = {}; // 0 == go left, 1 == go right
static Board boards[N*N] = {}; // ALCS boards w/ ranks

/*
> python3
>>> from humanize import naturalsize as ns
>>> print(" N |  Size\n------------"); [print(f"{N} | {ns(4 + 24 + 1 + N*4 + (2*N*N+7)//8 + N*N*2 + N*N*((N*N+7)//8) + N*N*(6*N - 2))}") for N in range(16,25)][0]
 N |  Size
------------
16 | 32.9 kB
17 | 40.3 kB
18 | 48.4 kB
19 | 57.9 kB
20 | 68.2 kB
21 | 80.4 kB
22 | 93.6 kB
23 | 108.6 kB
24 | 124.6 kB
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
  assert(N <= 24);
  assert(sizeof(Rank) == 1); // 1 byte
  assert(sizeof(Slot) == 2); // 2 bytes
  assert(sizeof(Board) == 2*bits(N*N)+(6*N-2)); // 2*N*N bits + 6N - 2 bytes
}

#endif /* TRANSAT_H_IMPLEMENTED */
