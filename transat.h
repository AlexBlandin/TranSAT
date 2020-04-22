#ifndef TRANSAT_H_IMPLEMENTED
#define TRANSAT_H_IMPLEMENTED

/* 22 is the max */
#ifndef N
#define N 4
#endif

/* n MUST BE A CONSTANT */
#define queen(n) if (n < N) queens.q##n
#define bd boards[board]
#define sl boards[board].slot
// #define rk ranks
#define rk boards[board].ranks
#define sl_dia (sl.row + sl.col)
#define sl_adg (N - sl.col + sl.row - 1)

typedef struct _Rank {
  u8 open: 5; /* how many spaces are open */
  u8 forbidden: 5; /* how many are locked out */
  u8 placed: 5; /* how many queens are there */
} Rank;

typedef struct _Ranks {
  Rank rows[N];
  Rank cols[N];
  Rank dias[2*N-1]; /* row + sl */
  Rank adia[2*N-1]; /* N - col + row - 1 */ /* Done this way to handle limited ranges */
  u32 open_rows; // bitset of open rows (0 = all full)
  u32 open_cols; // bitset of open columns
} Ranks;

typedef struct _Slot {
  i8 row; /* better to have separated row/col & multiply/add it (bc. FMA ops) than divide */
  i8 col;
} Slot;

#define OPEN 0
#define FORBIDDEN 1
#define PLACED 2

typedef struct _Board {
  u8 queens_left; /* how many pieces we have left to place */
  u16 visits; /* how many times this has been the current board on entry to the main loop */
  Slot slot; /* where we changed (either forbid or placed) */
  u8 state[N*N]; /* each space on the board, 0 = open, 1 = forbidden, 2 = placed queen */
  Ranks ranks; // taken out since we recompute each time so don't need storage, can speed up later
} Board;


/* DATA */
static u64 nq = 0; /* solutions */
static s16 board = 0; /* current board */
static u8 progress[bits(N*N)]; /* 0 = go left, 1 = go right */
static Board boards[N*N+1]; /* ALCS boards w/ ranks */
// static Ranks ranks;

static u64 solutions[] = {1, 1, 0, 0, 2, 10, 4, 40, 92, 352,
                          724, 2680, 14200, 73712, 365596,
                          2279184, 14772512, 95815104, 666090624,
                          4968057848, 39029188884, 314666222712,
                          2691008701644, 24233937684440,
                          227514171973736, 2207893435808352,
                          22317699616364044, 234907967154122528};

/* update ranks according to a given space */
#define rerank(row, col) \
  u8 diag = row + col;\
  u8 adia = N - col + row - 1;\
  switch (bd.state[row*N + col]) { \
    case PLACED: \
    rk.rows[row].placed++; \
    rk.cols[col].placed++; \
    rk.dias[diag].placed++; \
    rk.adia[adia].placed++; \
    break; \
    case FORBIDDEN: \
    rk.rows[row].forbidden++; \
    rk.cols[col].forbidden++; \
    rk.dias[diag].forbidden++; \
    rk.adia[adia].forbidden++; \
    break; \
    case OPEN: \
    rk.rows[row].open++; \
    rk.cols[col].open++; \
    rk.dias[diag].open++; \
    rk.adia[adia].open++; \
    rk.open_rows |= 1 << row; \
    rk.open_cols |= 1 << col; \
    break; \
  }

void init() {
  seed_rng();
  for (board = N*N; board--;) {
    for (u16 i = 0; i < N*N; i++)
      bd.state[i] = OPEN; /* in case we change it (unlikely) */
    bd.queens_left = N;
  }
  board = 0;

  /* compute the ranks */
  zero(rk);
  for (u8 i = 0; i < N; i++) {
    for (u8 j = 0; j < N; j++) {
      rerank(i, j);
    }
  }

  assert(N);
  assert(N <= 21);
  assert(sizeof(Rank));
  assert(sizeof(Ranks));
  assert(sizeof(Board) * N*N);
}

#endif /* TRANSAT_H_IMPLEMENTED */
