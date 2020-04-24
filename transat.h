#ifndef TRANSAT_H_IMPLEMENTED
#define TRANSAT_H_IMPLEMENTED

/* 22 is the max */
#ifndef N
#define N 21
#endif

#define bd boards[board]
#define sl boards[board].slot
#define rk boards[board].ranks

typedef struct _Rank {
  u8 open: 6; /* how many spaces are open */
  u8 placed: 2; /* already placed in */
} Rank; // bitwise ranks thing & propagation?

typedef struct _Ranks { /* for the number */
  Rank rows[N];
  Rank cols[N];
  Rank dias[2*N-1]; /* row + sl */
  Rank adia[2*N-1]; /* N - col + row - 1 */ /* Done this way to handle limited ranges */

  u32 n_open; /* how many are still open at all? */

  /* a 1 means that row/col/diagonal is open/forbid/placed */
  u32 open_rows;   /* which rows are open */
  u32 open_cols;   /* which cols are open */
  // u32 placed_rows; /* which rows are placed in */
  // u32 placed_cols; /* which cols are placed in */
  // u64 open_dias;   /* which dias are open */
  // u64 open_adga;   /* which adgs are open */
  // u64 placed_dias; /* which dias are placed in */
  // u64 placed_adga; /* which adgs are placed in */
} Ranks; /* TODO: SPEEDUP SERIOUSLY THIS IS THE SLOWEST THING */

typedef struct _Slot {
  u8 row; /* better to have separated row/col than having to always divide */
  u8 col;
  u8 dia; /* row + col */
  u8 adg; /* N - col + row - 1 */
} Slot;

static inline Slot slot(u8 row, u8 col) {
  return (Slot) {row, col, row + col, N - col + row - 1};
} 

#define at(row, col) (bd.state[(row)] & (1 << (col)))
#define set(row, col) (bd.state[(row)] |= (1 << (col)))
#define open(row, col) (bd.state[(row)] &= ~(1 << (col)))
#define is_open(row, col) (!at(row, col))

typedef struct _Board {
  u8 queens_left; /* how many pieces we have left to place */
  u16 visits; /* how many times this has been the current board on entry to the main loop */
  Slot slot; /* where we changed (either forbid or placed) */
  u16 slot_index; /* to be used if it is needed */
  u32 state[N]; /* each row on the board as 32bit columns, 0 = open, 1 = forbidden or placed in */
  Ranks ranks; // taken out since we recompute each time so don't need storage, can speed up later
} Board;

/* DATA */
static u64 nq = 0; /* solutions */
static s16 board = 0; /* current board */
static Board boards[N+1]; /* ALCS boards w/ ranks */
static Slot square_enum[N*N];
bool pb = false; /* preempted backtrack */
bool forced = false;
Slot queued;

/* move along the stack */
static inline void new_board() {
  copy(sizeof(Board), boards[board], boards[board+1]);
  board++;
  bd.visits = 0; // all new board have 0 visits
}

static u64 solutions[] = {1, 1, 0, 0, 2, 10, 4, 40, 92, 352,
                          724, 2680, 14200, 73712, 365596,
                          2279184, 14772512, 95815104, 666090624,
                          4968057848, 39029188884, 314666222712,
                          2691008701644, 24233937684440,
                          227514171973736, 2207893435808352,
                          22317699616364044, 234907967154122528};


/* reduce open ranks for a given space */
static inline void derank(u8 row, u8 col) {
  u8 diag = row + col;
  u8 adia = N - col + row - 1;
  rk.rows[row].open--;
  rk.cols[col].open--;
  rk.dias[diag].open--;
  rk.adia[adia].open--;
  rk.n_open--;
}

/* reduce open ranks for the current slot */
static inline void derank_sl() {
  rk.rows[sl.row].open--;
  rk.cols[sl.col].open--;
  rk.dias[sl.dia].open--;
  rk.adia[sl.adg].open--;
  rk.n_open--;
}

static inline void placed_sl() {
  rk.rows[sl.row].placed++;
  rk.cols[sl.col].placed++;
  rk.dias[sl.dia].placed++;
  rk.adia[sl.adg].placed++;
}

/* clear a full row */
static inline void cf_row(u8 row) {
  rk.open_rows &= ~((rk.rows[row].open == 0) << row); // is if() rk... faster?
}

/* clear a full col */
static inline void cf_col(u8 col) {
  rk.open_cols &= ~((rk.cols[col].open == 0) << col); // is if() rk... faster?
}

/* clear a full row and col */
static inline void clear_full(u8 row, u8 col) {
  cf_row(row); cf_col(col);
}

/* recompute ranks from scratch according to a given space */
static inline void rerank(u16 row, u16 col) {
  u16 diag = row + col;
  u16 adia = N - col + row - 1;
  if (at(row, col)) {
    rk.rows[row].placed++;
    rk.cols[col].placed++;
    rk.dias[diag].placed++;
    rk.adia[adia].placed++;
  } else {
    rk.rows[row].open++;
    rk.cols[col].open++;
    rk.dias[diag].open++;
    rk.adia[adia].open++;
    rk.n_open++;
    rk.open_rows |= 1 << row;
    rk.open_cols |= 1 << col;
  }
}

void init() {
  seed_rng();
  for (board = N; board--;) {
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
  assert(sizeof(Board));
  assert(sizeof(boards));
}

#endif /* TRANSAT_H_IMPLEMENTED */
