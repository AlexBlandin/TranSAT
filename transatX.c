/* COPYRIGHT Alex Blandin (c) 2020 */
/* See LICENSE */

/* transat except extint */

/* #include <https://raw.githubusercontent.com/AlexBlandin/short.hand/main/s.h> */ /* godbolt.org magic */
#include <s.h>

#define N 12

#define Bits _ExtInt
typedef Bits(N) row_t;
typedef Bits(2*N-1) diag_t;
typedef Bits(3*N) exrow_t;
typedef Bits(N*N) board_t;

const row_t full_row = -1;
#define space_left(rowcol) ((rowcol) != full_row)

static inline exrow_t embed(row_t row) { return ((exrow_t)row) << N; }
static inline row_t ex_truncate(exrow_t exrow) { return (row_t)(exrow >> N); }
static inline exrow_t add(row_t row, exrow_t exrow) { return exrow | (((exrow_t)row) << N); }

#define bd stack[board]
#define sl bd.slot
#define rk bd.ranks
#define index(row, col) ((row)*N + (col))
#define diagonal(row, col) ((row) + (col))
#define antidiagonal(row, col) (N - (col) + (row)-1)

/* given an N*N bitset and offset to the first bit, grab N bits worth */
/* Reliant on ExtInt types lopping off the higher bits */
#define access(bitset, offset) ((row_t)((bitset) >> (N*N-(offset)-N)))

// @TEMP, will rewrite to Bits setup, which will be looking up in a board_t
#define at(row, col) (bd.rows[(row)] & (row_t)(1 << (col)))
#define set(row, col) ((bd.rows[(row)] |= (row_t)(1 << (col))), (bd.cols[(col)] |= (row_t)(1 << (row))))
#define open(row, col) (not at(row, col))
#define open_sl(s) (open((s).row, (s).col))
#define lut_open(i) (open(lut[i].row, lut[i].col))

/* given bd.rows[row] or bd.cols[col] */
#define n_open(rowcol) (N - bitcount32((rowcol)))

/* given bd.rows[row] or bd.cols[col] with a forced move, find it */
#define forced(rowcol) (which_bit(~(rowcol)))

typedef struct _Slot {
  u8 row; /* better to have separated row/col than having to always divide */
  u8 col;
  u8 dia; /* diagonal(row, col) */
  u8 adg; /* antidiagonal(row, col) */
} Slot;

static inline bool peaceful(Slot a, Slot b) {
  return (a.row == b.row and a.col == b.col) or
   (a.row != b.row and a.col != b.col and a.dia != b.dia and a.adg != b.adg);
}

typedef struct _Ranks { /* how many spaces are open */
  u8 dias[2 * N - 1];   /* row + col */
  u8 adia[2 * N - 1];   /* N - col + row - 1, done this way to handle limited ranges */
} Ranks;

static inline Slot slot(u8 row, u8 col) {
  return (Slot) {row, col, diagonal(row, col), antidiagonal(row, col)};
}

/* ACLS Chessboard */
typedef struct _Board {
  u8 queens_left; /* how many pieces we have left to place */
  u16 i;          /* where we placed (tyically row*N+col, for lookup in LUT) */
  Slot slot;
  bool falsified;
  u32 open;      /* how many spaces are open */
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
}

static inline Slot heuristic() {
  Slot s = {0, 0, 0, 0};
  /* from 0,0 indices */
  for (; bd.i < N * N; bd.i++)
    if (lut_open(bd.i))
      return lut[bd.i];
  return s;
}

/*
  this is where we'd have TAW heuristic stuff, but that can wait 
  the focus is on getting _ExtInt in for N bit row_t and N*N bit board_t
*/


/* is this board solved / trivial to solve now? */
static inline bool satisfied() {
  if (bd.queens_left == 1) {
    /* if there's any open they are all legal */
    nq += bd.open;
    return true;
  }
  return false;
}

/* is this board valid / usable for further queen placement? */
static inline bool falsified() { return bd.falsified or bd.open == 0 or bd.queens_left == 0; }

/* for the current slot, AMO propagate */
static inline void prop_amo(Slot s) {
  /* AMO propagate over row and update ranks */
  for (u8 col = 0; space_left(bd.rows[s.row]) and col < N; col++) {
    if (open(s.row, col)) {
      set(s.row, col);
      bd.open--;
      derank(s.row, col);
    }
  }
  /* AMO propagate over column and update ranks */
  for (u8 row = 0; space_left(bd.cols[s.col]) and row < N; row++) {
    if (open(row, s.col)) {
      set(row, s.col);
      bd.open--;
      derank(row, s.col);
    }
  }

  /* AMO propagate outwards over diagonals and update ranks */
  for (u8 i = 1; (rk.dias[s.dia] or rk.adia[s.adg]) and i < N; i++) {
    u8 up = s.col - i;
    u8 down = s.col + i;
    u8 left = s.row - i;
    u8 right = s.row + i;

    /* we're using underflow to our advantage */
    if (left < N and up < N and open(left, up)) {
      set(left, up); /* rk.dias[diagonal(left, up)].open */
      bd.open--;
      derank(left, up);
    }
    if (left < N and down < N and open(left, down)) {
      set(left, down); /* rk.adia[antidiagonal(left, down)].open */
      bd.open--;
      derank(left, down);
    }
    if (right < N and up < N and open(right, up)) {
      set(right, up); /* rk.adia[antidiagonal(right, up)].open */
      bd.open--;
      derank(right, up);
    }
    if (right < N and down < N and open(right, down)) {
      set(right, down); /* rk.dias[diagonal(right, down)].open */
      bd.open--;
      derank(right, down);
    }
  }
}

/* for the current board, ALO propagate */
static inline void prop_alo() {
  static Slot queue[4] = {};
  s8 have_placed = -1; /* indices 0 to 3 in the queue or -1 (empty queue) */

  /* ALO propagate over rows/cols */
  for (u8 rowcol = 0; not falsified() and rowcol < N; rowcol++) {
    if (space_left(bd.rows[rowcol]) and n_open(bd.rows[rowcol]) == 1) {
      queue[++have_placed] = slot(rowcol, forced(bd.rows[rowcol]));
    }
    if (space_left(bd.cols[rowcol]) and n_open(bd.cols[rowcol]) == 1) {
      queue[++have_placed] = slot(forced(bd.cols[rowcol]), rowcol);
    }
  }

  if (not falsified()) {
    for (; have_placed > -1; have_placed--) {
      if (open_sl(queue[have_placed])) {
        occupy(queue[have_placed]);
        bd.queens_left--;
        prop_amo(queue[have_placed]);
      } else {
        bd.falsified = true; /* the space has been closed since, meaning there is a Queen that can capture this */
      }
    }
  }
  
  if (bd.queens_left == 0 and not bd.falsified and bd.open) nq++;
}

static inline void prop_closure() {
  u32 prev_open; /* we can tell if propagation has occured as the number of open slots will have decreased */
  /* AMO propagate */
  prop_amo(sl);
  // prop_alo();

  // do {
  //   prev_open = bd.open;
  //   /* ALO propagate */
  //   prop_alo();
  // } while(not falsified() and bd.open != prev_open); /* until falsified or it stops propagating */
}

/* the TranSAT N-Queens solver */
static inline void transat() {
  while (board > -1) { /* starts at 0 */

    if (falsified() or satisfied()) {
      board--;
    } else {
      /* select branching variable (the slot/space we're focusing on) */
      sl = heuristic(); /* always chooses an open slot, so no need to worry about that */

      /* forbid a space */
      occupy(sl);
      // prop_alo();

      /* place a queen on a new board */
      board++;                         /* move along the stack */
      stack[board] = stack[board - 1]; /* clone the board */
      // copy(sizeof(Board), stack[board-1], stack[board]);
      bd.queens_left--;

      prop_closure();
    }
  }
}

int main() {
  init();
  for (u16 i = 0; i < N * N; i++) {
    lut[i] = slot(i / N, i % N);
  }
  if (N <= 1) {
    nq = 1;
  } else {
    transat();
  }

  if (nq == solutions[N]) /* addressed by N as N=0 is included */
    printf(N > 9 ? "%d | " : " %d | ", N);
  else
    printf("Q(%d) gave %lu, should be %lu\n", N, nq, solutions[N]);
  return nq != solutions[N];
}
