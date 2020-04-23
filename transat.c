// /* https://github.com/nothings/stb */
// #include "stretchy_buffer.h" /* requires -Wno-unused-value */

#include "s.h" /* short.hand */
#include "transat.h" /* transat header (types, common funcs, etc) */


/* pick a space any (open) space */
static inline Slot heuristic() {
  /* pick a heuristic */
  #define FIRSTROW

  #if defined(FIRSTROW)
  /* from 0,0 indices */
  for (u8 row = 0; row < N; row++)
    for (u8 col = 0; col < N; col++)
      if open(row, col)
        return (Slot) { row, col, row + col, N-col+row-1 };
  #elif defined(FIRSTROW_BK)
  for (u8 row = N-1; row < N; row--)
    for (u8 col = N-1; col < N; col--)
      if open(row, col)
        return (Slot) { row, col, row + col, N-col+row-1 };
  #elif defined(SQUAREENUM)

  #elif defined(TAW)
  #elif defined(ANTITAW)
  #else
  #error "You need to choose a heuristic"
  #endif
  return (Slot) {0, 0, 0, N-1};
}

/* is this board solved / trivial to solve now? */
static bool satisfied() {
  u64 prev_nq = nq; /* previous queens count */
  switch (bd.queens_left) {
    case 0:
    /* only valid n-queens configs make it this far */
    nq++;

    break;
    case 1:
    /* propagation */

    break;
    default:
    break;
  }
  return nq > prev_nq;
}

/* is this board valid / usable for further queen placement? */
static inline bool falsified() {
  return rk.open_rows == 0 and rk.open_cols == 0; /* ALO unsatisfiable, as AMO sat is guaranteed by heuristic generation */
}

/* the TranSAT N-Queens solver */
static inline void transat() {
  bool pb = false; /* preempted backtrack */
  bool forced = false;
  Slot queued = (Slot) {0, 0, 0, N-1};
  do {
    assert(board <= N);
    bd.visits++;

    #ifdef PRINTOUT
    if (bd.queens_left == 0) {
      printf("b[%d], ql = %d, sl = (%d, %d), nq = %"LU"\n", board, bd.queens_left, sl.row, sl.col, nq);
      for (u16 i = 0; i < N; i++) {
        for (u16 j = 0; j < N; j++)
          printf("%d ", at(i*N + j]);
        println();
      }
      println();
    }
    #endif

    if (not pb and (satisfied() or falsified())) {
      board--;
      continue;
    }

    /* odd is placed, even is forbid */
    if (bd.visits & 1) {
      /* select branching variable (the slot/space we're focusing on) */
      if (forced) {
        sl = queued;
        forced = false;
      } else {
        sl = heuristic(); /* always chooses an open slot, so no need to worry about that */
        if (rk.rows[sl.row].placed or rk.cols[sl.col].placed or rk.dias[sl.dia].placed or rk.adia[sl.adg].placed) {
          pb = true; /* AMO unsatisfied, so we're going to loop around and immediately forbid it */
          continue;
        }
      }
      copy(sizeof(Board), boards[board], boards[board+1]);
      board++;
      bd.visits = 0; // all new board have 0 visits

      /* place a queen */
      at(sl.row, sl.col) = PLACED;
      bd.queens_left--;
      placed_sl();
      derank_sl();

      /* propagate over row and update ranks */
      for (u8 j = 0; j < sl.col; j++) {
        if (open(sl.row, j)) {
          at(sl.row, j) = FORBIDDEN;
          derank(sl.row, j);
          cf_col(j);
        }
      } for (u8 j = sl.col + 1; j < N; j++) {
        if (open(sl.row, j)) {
          at(sl.row, j) = FORBIDDEN;
          derank(sl.row, j);
          cf_col(j);
        }
      }
      /* propagate over column and update ranks */
      for (u8 i = 0; i < sl.row; i++) {
        if (open(i, sl.col)) {
          at(i, sl.col) = FORBIDDEN;
          derank(i, sl.col);
          cf_row(i);
        }
      } for (u8 i = sl.row + 1; i < N; i++) {
        if (open(i, sl.col)) {
          at(i, sl.col) = FORBIDDEN;
          derank(i, sl.col);
          cf_row(i);
        }
      }
      clear_full(sl.row, sl.col);

      /* propagate (AMO) over diagonals and update ranks */
      for (u8 d = 1; d < N; d++) {
        u8 r1 = sl.row+d; u8 c1 = sl.col+d;
        u8 r2 = sl.row+d; u8 c2 = sl.col-d;
        u8 r3 = sl.row-d; u8 c3 = sl.col-d;
        u8 r4 = sl.row-d; u8 c4 = sl.col+d;
        if (r1 < N and c1 < N) {
          if open(r1, c1) {
            at(r1, c1) = FORBIDDEN;
            derank(r1, c1);
            clear_full(r1, c1);
          }
        }

        if (r2 < N and c2 < N) {
          if open(r2, c2) {
            at(r2, c2) = FORBIDDEN;
            derank(r2, c2);
            clear_full(r2, c2);
          }
        }

        if (r3 < N and c3 < N) {
          if open(r3, c3) {
            at(r3, c3) = FORBIDDEN;
            derank(r3, c3);
            clear_full(r3, c3);
          }
        }

        if (r4 < N and c4 < N) {
          if open(r4, c4) {
            at(r4, c4) = FORBIDDEN;
            derank(r4, c4);
            clear_full(r4, c4);
          }
        }
      }
    } else {
      /* forbid a space */
      at(sl.row, sl.col) = FORBIDDEN;
      derank_sl();
      clear_full(sl.row, sl.col);

      /* ALO propagation (forced move) */
      if (rk.rows[sl.row].open - 1 == 1) { // if, after closing a slot, there is only 1 open, it's a forced move
        for (u8 i = 0; i < N; i++) {
          if open(sl.row, i) {
            forced = true;
            queued = (Slot) {sl.row, i, sl.row + i, N - sl.col + sl.row - 1}; // queue a forced move from the same row for the next loop
            break;
          }
        }
      }
      if (not forced and rk.cols[sl.col].open - 1 == 1) {
        for (u8 i = 0; i < N; i++) {
          if open(i, sl.col) {
            forced = true;
            queued = (Slot) {i, sl.col, i + sl.col, N - sl.col + i - 1}; // queue a forced move from the same col for the next loop
            break;
          }
        }
      }
    }
  } while(board > -1);
}

int main() {
  init();

  transat();

  if (nq == solutions[N]) /* addressed by N as N=0 is included */
    printf("Q(%d) = %"LU"\n", N, nq);
  else
    printf("Q(%d) gave %"LU", should be %"LU"\n", N, nq, solutions[N]);
  return nq != solutions[N];
}
