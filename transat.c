// /* https://github.com/nothings/stb */
// #include "stretchy_buffer.h" /* requires -Wno-unused-value */

#include "s.h" /* short.hand */
#include "transat.h" /* transat header (types, common funcs, etc) */


/* pick a space any (open) space */
static inline Slot heuristic() {
  /* pick a heuristic */
  #define FIRSTOPEN

  #if defined(FIRSTOPEN)
  for (u16 i = sl.row * sl.col; i < N*N; i++)
    if (bd.state[i] == OPEN)
      return (Slot) { (i/N), (i%N) };
  #elif defined(FIRSTROW)
  #elif defined(SQUAREENUM)
  #elif defined(TAW)
  #elif defined(ANTITAW)
  #else
  #error "You need to choose a heuristic"
  #endif
  return (Slot){0, 0};
}

/* is this board trivial to solve now? */
static bool satisfied() {
  u64 prev_nq = nq; /* previous queens count */
  u8 queens = 0;
  switch (bd.queens_left) {
    case 0:
    /* for all valid n-queens configs */
    for (u16 i = 0; i < N; i++) // can speedup later, less pressing than the others
      for (u16 j = 0; j < N; j++)
        if (rk.rows[i].placed > 1 or rk.cols[j].placed > 1 or rk.dias[i+j].placed > 1 or rk.adia[N-j+i-1].placed > 1)
          return false;
    nq++;
    break;
    // case 1: // this seems to be wrong
    // /* for all forced assignments, nq++ */
    // for (u8 i = 0; i < N; i++) {
    //   if (rk.rows[i].open == 1) {
    //     for (u8 j = 0; j < N; j++) {
    //       if (rk.cols[j].open == 1 and
    //           rk.dias[i+j].open == 1 and
    //           rk.adia[N-j+i-1].open == 1 and
    //           bd.state[i*N + j] == OPEN and
    //           rk.cols[j].placed <= 1 and
    //           rk.dias[i+j].placed <= 1 and
    //           rk.adia[N-j+i-1].placed <= 1) {
    //           //(bs_in(bd.open, i))) {
    //           nq++;
    //       }
    //     }
    //   }
    // }
    // break;
    default:
    break;
  }
  return nq != prev_nq;
}

/* is this board valid / usable for further queen placement? */
static bool falsified() {
  /* ALO unsatisfiable */
  if (rk.open_rows == 0 and rk.open_cols == 0)
    return true;

  /* AMO unsatisfied */
  /* only do if using a heuristic that can give AMO unsatisfiable output */
  #if defined(FIRSTOPEN) or defined(FIRSTROW) or defined(SQUAREENUM) or defined(TAW) or defined(ANTITAW)
  for (u16 i = 0; i < N; i++)
    for (u16 j = 0; j < N; j++)
      if (rk.rows[i].placed > 1 or rk.cols[j].placed > 1 or rk.dias[i+j].placed > 1 or rk.adia[N-j+i-1].placed > 1)
        return true;
  #endif

  return false;
}


/*
  TODO: speedup all ranks, they're by far the slowest thing
*/

/* the TranSAT N-Queens solver */
static inline void transat() {
  bool forced = false;
  Slot queued = (Slot){0,0};
  do {
    assert(board <= N);
    bd.visits++;

    #ifdef PRINTOUT
    if (bd.queens_left == 0) {
      printf("b[%d], ql = %d, sl = (%d, %d), nq = %"LU"\n", board, bd.queens_left, sl.row, sl.col, nq);
      for (u16 i = 0; i < N; i++) {
        for (u16 j = 0; j < N; j++)
          printf("%d ", bd.state[i*N + j]);
        println();
      }
      println();
    }
    #endif

    if (satisfied() or falsified()) {
      board--;
      continue;
    }

    /* odd is placed, even is forbid */
    if (bd.visits & 1) {

      // TODO: I know the old ranks and slot at this point

      /* select branching variable (the slot/space we're focusing on) */
      if (forced) {
        sl = queued;
        forced = false;
      } else {
        sl = heuristic();
      }

      // TODO: I now know the new slot

      copy(sizeof(Board), boards[board], boards[board+1]);
      board++;
      bd.visits = 0; // all new board have 0 visits

      /* place a queen */
      bd.state[sl.row*N + sl.col] = PLACED;
      bd.queens_left--;

      // TODO: I have placed the queen and can start to update the ranks

      /* propagate over row/column AMO */
      for (u16 i = 0; i < sl.row; i++)
        bd.state[i*N + sl.col] = FORBIDDEN;
      for (u16 i = 0; i < sl.col; i++)
        bd.state[sl.row*N + i] = FORBIDDEN;
      for (u16 i = sl.row + 1; i < N; i++)
        bd.state[i*N + sl.col] = FORBIDDEN;
      for (u16 i = sl.col + 1; i < N; i++)
        bd.state[sl.row*N + i] = FORBIDDEN;

      // TODO: I have done rooks propogatation

      /* propagate over diagonal/antidiagonal AMO */
      for (s8 i = 0; i < N; i++)
        for (s8 j = 0; j < N; j++)
          if ((i + j == sl.row + sl.col or i - j == sl.row - sl.col) and i != sl.row and j != sl.col)
            bd.state[i*N + j] = FORBIDDEN;

      // TODO: I have done bishops propogation

      /* recompute the ranks */ // TODO: speedup <- the ranks are the slowest thing here
      zero(rk);
      for (u8 i = 0; i < N; i++) {
        for (u8 j = 0; j < N; j++) { // we can make this faster using bitcounts of 3 bitsets
          rerank(i, j);
        }
      }

      // TODO: I am looping, next visit at this level is already incremental

    } else {
      /* forbid a space */
      bd.state[sl.row*N + sl.col] = FORBIDDEN;
      rk.rows[sl.row].forbidden++;
      rk.cols[sl.col].forbidden++;
      rk.dias[sl_dia].forbidden++;
      rk.adia[sl_adg].forbidden++;
      rk.rows[sl.row].open--;
      rk.cols[sl.col].open--;
      rk.dias[sl_dia].open--;
      rk.adia[sl_adg].open--;
      if (rk.rows[sl.row].open == 0)
        rk.open_rows &= ~(1 << (u32) sl.row);
      if (rk.cols[sl.col].open == 0)
        rk.open_cols &= ~(1 << (u32) sl.col);

      /* ALO propagation (forced move) */
      if (rk.rows[sl.row].open - 1 == 1){ // if, after closing a slot, there is only 1 open, it's a forced move
        for (u8 i = 0; i < N; i++) {
          if (bd.state[sl.row*N + i] == OPEN) {
            forced = true;
            queued = (Slot){sl.row, i}; // queue the forced move for the next loop
            break;
          }
        }
      }
      if (not forced and rk.cols[sl.col].open - 1 == 1) {
        for (u8 i = 0; i < N; i++) {
          if (bd.state[i*N + sl.col] == OPEN) {
            forced = true;
            queued = (Slot){i, sl.col};
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
