#ifndef TRANSAT_H_IMPLEMENTED
#define TRANSAT_H_IMPLEMENTED

// 22 is the max
#ifndef N
#define N 4
#endif

// n MUST BE A CONSTANT
#define queen(n) if (n < N) queens.q##n
#define bd boards[board]
#define sl boards[board].slot
#define rk boards[board].ranks
#define sl_dia (sl.row + sl.col)
#define sl_adg (N - sl.col + sl.row - 1)

typedef union _Rank {
  struct {
  u8 placed: 1; // AMO means only one so this can be a bit
  u8 forbidden: 5; // how many are locked out
  u8 open: 5;
  };
  u16 rank; // I personally always have a union over bitfields in case I need to address the entire thing
} Rank;

typedef struct _Ranks {
  Rank rows[N]; // how many are placed/forbidden
  Rank cols[N]; // (open = N - closed, closed = placed + forbidden))...
  Rank dias[2*N-1]; // diagonal open replaces N with the diagonal length
  Rank adia[2*N-1]; // open = len - closed, len = N - abs(row-col) FOR ADIA NOT DIAS
} Ranks;

typedef struct _Slot {
  i8 row; // better to have separated row/col & multiply/add it (bc. FMA ops) than divide
  i8 col;
} Slot;

#define as_slot(i) ((Slot) { (i/N), (i%N) })

#define OPEN 0
#define FORBIDDEN 1
#define PLACED 2

typedef struct _Board {
  // u8 forbid[bits(N*N)]; // 0 = not forbidden, 1 = forbidden
  // u8 placed[bits(N*N)]; // 0 = no queen, 1 = queen
  // u8 open[bits(N*N)]; // 0 = not open, 1 = open
  u8 queens; // how many queens we have
  Slot slot; // where we changed (either forbid or placed)
  u8 state[N*N]; // state, 0 = open, 1 = forbidden, 2 = placed queen
  Ranks ranks;
} Board;


/* DATA */
static u64 nq = 0; // solutions
static u16 board = 0; // current board
static u8 progress[bits(N*N)]; // 0 = go left, 1 = go right
static u8 backtrack[bits(N*N)]; // 0 = deepen, 1 = backtrack
static Board boards[N*N+1]; // ALCS boards w/ ranks

/*
> python3
from humanize import naturalsize as ns
print("\n N |  Size   | Maybe\n---------------------"); [print(f"{N} | {ns(x:=0*N+8+2+(N*N+7)//8+N*N*(3+2*(N*N+7)//8+(N+N+2*(2*N-1)+2*N-1)))} | {ns(x/N)}") for N in range(16,25)][0]

 N |  Size   | Maybe (given I can take out a factor of N via the forbids...)
---------------------
16 | 41.5 kB | 2.6 kB
17 | 51.2 kB | 3.0 kB
18 | 61.9 kB | 3.4 kB
19 | 74.8 kB | 3.9 kB
20 | 88.9 kB | 4.4 kB
21 | 105.5 kB | 5.0 kB
22 | 123.5 kB | 5.6 kB
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
  for (board = N*N; board--;) {
    for (u16 j = 0; j < N; j++)
      rk.rows[j].open = rk.cols[j].open = N;
    for (u16 j = 0; j < (2*N-1); j++)
      rk.dias[j].open = rk.adia[j].open = N;
    for (u16 j = 0; j < N*N; j++)
      bd.state[j] = OPEN;
    // for (u16 j = 0; j < bits(N*N); j++)
    //   bd.open[j] = -1;
  }
  board = 0;

  assert(N);
  assert(N <= 21);
  assert(sizeof(Board) * N*N);
}

#endif /* TRANSAT_H_IMPLEMENTED */
