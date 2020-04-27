#ifndef TRANSAT_H_IMPLEMENTED
#define TRANSAT_H_IMPLEMENTED

/* N <= 21 */
#ifndef N
#define N 21
#endif

/* simple bitset for N-Queens row-like structures */
typedef u32 row_t;
/* extended bitset for up to 3*N (assuming N <= 21) */
typedef u64 exrow_t;

const row_t full_row = (1 << N) - 1;

static inline exrow_t embed(row_t row) { return ((exrow_t) row) << N; }
static inline row_t truncate(exrow_t exrow) { return (row_t) (exrow >> N); }
static inline exrow_t add(row_t row, exrow_t exrow) { return exrow | (((exrow_t) row) << N); }

#define bd boards[board]
#define sl bd.slot
#define rk boards[board].ranks
#define index(row, col) (row*N + col)
#define diagonal(row, col) ((row) + (col))
#define antidiagonal(row, col) (N - (col) + (row) - 1)

#define at(row, col) (bd.rows[(row)] & (1 << (col)))
#define set(row, col) ((bd.rows[(row)] |= (1 << (col))), (bd.cols[(col)] |= (1 << (row))))
#define open(row, col) (not at(row, col))
#define lut_open(i) (open(lut[i].row, lut[i].col))

typedef struct _Slot {
  u8 row; /* better to have separated row/col than having to always divide */
  u8 col;
  u8 dia; /* diagonal(row, col) */
  u8 adg; /* antidiagonal(row, col) */
} Slot;

typedef struct _Ranks {
  // u8 rows[N]; /* how many spaces are open */
  // u8 cols[N];
  u8 dias[2*N-1]; /* row + sl */
  u8 adia[2*N-1]; /* N - col + row - 1 */ /* Done this way to handle limited ranges */

  /* a 1 means that row/col/diagonal is open/forbid/placed */
  row_t open_rows; /* which rows are open */
  row_t open_cols; /* which cols are open */
} Ranks;

static inline Slot slot(u8 row, u8 col) {
  return (Slot) { row, col, diagonal(row, col), antidiagonal(row, col) };
}

/* ACLS Chessboard */
typedef struct _Board {
  u8 queens_left; /* how many pieces we have left to place */
  u16 i; /* where we placed (tyically row*N+col, for lookup in LUT) */
  Slot slot;
  u32 open; /* how many spaces are open */
  u16 visits; /* how many times this has been the current board on entry to the main loop */
  row_t rows[N]; /* each row on the board as 32bit columns, 0 = open, 1 = forbidden or placed in */
  row_t cols[N]; /* transposed */
  Ranks ranks;
} Board;

/* DATA */
static u64 nq = 0; /* solutions */
static s8 board = 0; /* current board */
static Board boards[N]; /* ALCS boards w/ ranks */
static Slot lut[N*N]; /* 0..N*N-1 to slot LUT */
static Slot queued; /* queued index */
bool is_queued = false; /* if a slot has been queued up */

static u64 solutions[] = {1, 1, 0, 0, 2, 10, 4, 40, 92, 352,
                          724, 2680, 14200, 73712, 365596,
                          2279184, 14772512, 95815104, 666090624,
                          4968057848, 39029188884, 314666222712,
                          2691008701644, 24233937684440,
                          227514171973736, 2207893435808352,
                          22317699616364044, 234907967154122528};

/* move along the stack */
static inline void copy_board() {
  copy(sizeof(Board), boards[board], boards[board+1]);
  board++;
  bd.visits = 0; // all new board have 0 visits
}

/* reduce open ranks for a given space */
static inline void derank(u8 row, u8 col) {
  u8 dia = row + col;
  u8 adg = antidiagonal(row, col);
  // rk.rows[row]--;
  // rk.cols[col]--;
  rk.dias[dia]--;
  rk.adia[adg]--;
}

/* reduce open ranks for the current slot */
static inline void derank_sl() {
  // rk.rows[sl.row]--;
  // rk.cols[sl.col]--;
  rk.dias[sl.dia]--;
  rk.adia[sl.adg]--;
}

/* clear a full row */
static inline void cf_row(u8 row) {
  rk.open_rows &= ~((bd.rows[row] == full_row) << row);
}

/* clear a full col */
static inline void cf_col(u8 col) {
  rk.open_cols &= ~((bd.cols[col] == full_row) << col);
}

/* clear a full row and col */
static inline void clear_full(u8 row, u8 col) {
  cf_row(row); cf_col(col);
}

/* recompute ranks from scratch according to a given space */
static inline void rerank(Slot s) {
  if open(s.row, s.col) {
    // rk.rows[s.row]++;
    // rk.cols[s.col]++;
    rk.dias[s.dia]++;
    rk.adia[s.adg]++;
    rk.open_rows |= 1 << s.row;
    rk.open_cols |= 1 << s.col;
  }
}

/* initialise boards and ranks */
void init() {
  seed_rng();
  for (board = N; board--;) {
    bd.queens_left = N;
    bd.open = N*N;
  }
  board = 0;

  /* compute the ranks */
  zero(rk);
  for (u8 i = 0; i < N; i++) {
    for (u8 j = 0; j < N; j++) {
      rerank(slot(i, j));
    }
  }

  assert(N);
  assert(N <= 21); // going over requires threading & other work
  assert(sizeof(Ranks));
  assert(sizeof(Board));
  assert(sizeof(boards));
}

#endif /* TRANSAT_H_IMPLEMENTED */
