#ifndef TRANSAT_H_IMPLEMENTED
#define TRANSAT_H_IMPLEMENTED

#include "s.h"       /* short.hand */

/* N <= 21 */
#ifndef N
#define N 21
#endif

/* simple bitset for N-Queens row-like structures */
typedef u32 row_t;
/* extended bitset for up to 3*N (assuming N <= 21) */
typedef u64 exrow_t;

const row_t full_row = (1 << N) - 1;
#define space_left(rowcol) ((rowcol) != full_row)

static inline exrow_t embed(row_t row) { return ((exrow_t)row) << N; }
static inline row_t truncate(exrow_t exrow) { return (row_t)(exrow >> N); }
static inline exrow_t add(row_t row, exrow_t exrow) { return exrow | (((exrow_t)row) << N); }

#define bd stack[board]
#define sl bd.slot
#define rk bd.ranks
#define index(row, col) ((row)*N + (col))
#define diagonal(row, col) ((row) + (col))
#define antidiagonal(row, col) (N - (col) + (row)-1)

/* given an N*N integral bitset and offset to the first value, return N bits worth */
/* PLACEHOLDER TO DEMONSTRATE, NO SUCH BITSET EXISTS IN STANDARD C */
#define access(bitset, offset) (bitset & (((1 << N) - 1) << (offset)))

#define at(row, col) (bd.rows[(row)] & (1 << (col)))
#define set(row, col) ((bd.rows[(row)] |= (1 << (col))), (bd.cols[(col)] |= (1 << (row))))
#define open(row, col) (not at(row, col))
#define open_sl(s) (open((s).row, (s).col))
#define lut_open(i) (open(lut[i].row, lut[i].col))

/* given bd.rows[row] or bd.cols[col] */
#define n_open(rowcol) (N - bitcount32((rowcol)))

/* given bd.rows[row] or bd.cols[col] with a forced move, find it */
#define forced(rowcol) (which_bit(~(rowcol)))

#define peaceful(a, b) \
  ((a.row == b.row and a.col == b.col) or \
   (a.row != b.row and a.col != b.col and a.dia != b.dia and a.adg != b.adg))

typedef struct _Slot {
  u8 row; /* better to have separated row/col than having to always divide */
  u8 col;
  u8 dia; /* diagonal(row, col) */
  u8 adg; /* antidiagonal(row, col) */
} Slot;

typedef struct _Ranks { /* how many spaces are open */
  u8 dias[2 * N - 1];   /* row + col */
  u8 adia[2 * N - 1];   /* N - col + row - 1, done this way to handle limited ranges */
} Ranks;

static inline Slot slot(u8 row, u8 col) {
  return (Slot){row, col, diagonal(row, col), antidiagonal(row, col)};
}

/* ACLS Chessboard */
typedef struct _Board {
  u8 queens_left; /* how many pieces we have left to place */
  u16 i;          /* where we placed (tyically row*N+col, for lookup in LUT) */
  Slot slot;
  bool falsified;
  u32 open;      /* how many spaces are open */
  row_t ballast[16]; /* optional ballast */
  row_t rows[N]; /* each row on the board as 32bit columns, 0 = open, 1 = forbidden or placed in */
  row_t cols[N]; /* transposed */
  Ranks ranks;
} Board;

/* DATA */
static u64 nq = 0;      /* solutions */
static s8 board = 0;    /* current board */
static Board stack[N];  /* ALCS boards w/ ranks */
static Slot lut[N * N]; /* 0..N*N-1 to slot LUT */

static u64 solutions[] = {1, 1, 0, 0, 2, 10, 4, 40, 92, 352,
                          724, 2680, 14200, 73712, 365596,
                          2279184, 14772512, 95815104, 666090624,
                          4968057848, 39029188884, 314666222712,
                          2691008701644, 24233937684440,
                          227514171973736, 2207893435808352,
                          22317699616364044, 234907967154122528};


static inline u32 odegree(Slot s) {
  return n_open(bd.rows[s.row]) + n_open(bd.cols[s.col]) + rk.dias[s.dia] + rk.adia[s.adg];
}

typedef struct _WeightPair {
  float first, second;
} WeightPair;
/* precomputed weights based on GenericSAT's version of tawSolver's heuristics */
static const float weights[] = {0,      0,      4.85,   1,      0.354,  0.11,   0.0694, 0.9740,
                                0.9488, 0.9242, 0.9002, 0.8769, 0.8542, 0.8320, 0.8104, 0.7894,
                                0.7690, 0.7490, 0.7296, 0.7107, 0.6923, 0.6743, 0.6569};
static inline WeightPair taw_weights(Slot s) {
  return (WeightPair) { odegree(s) * weights[2], weights[n_open(bd.rows[s.row])] + weights[n_open(bd.cols[s.col])] };
}

/* reduce open ranks for a given space */
static inline void derank(u8 row, u8 col) {
  rk.dias[diagonal(row, col)]--;
  rk.adia[antidiagonal(row, col)]--;
}

/* reduce open ranks for the current slot */
static inline void derank_sl(Slot s) {
  rk.dias[s.dia]--;
  rk.adia[s.adg]--;
}

/* forbids the current slot in the current board */
static inline void occupy(Slot s) {
  set(s.row, s.col);
  bd.open--;
  derank_sl(s);
}

/* initialise boards and ranks */
void init() {
  seed_rng();
  for (board = N; board--;) {
    bd.queens_left = N;
    bd.open = N * N;
  }
  board = 0;

  /* compute the ranks */
  zero(rk);
  for (u8 i = 0; i < N; i++) {
    for (u8 j = 0; j < N; j++) {
      Slot s = slot(i, j);
      if (open(s.row, s.col)) {
        rk.dias[s.dia]++;
        rk.adia[s.adg]++;
      }
    }
  }

  assert(N);
  assert(N <= 21); // going over requires threading & other work
  assert(sizeof(Ranks));
  assert(sizeof(Board));
  assert(sizeof(stack));
  assert(sizeof(lut));
}

#endif /* TRANSAT_H_IMPLEMENTED */
