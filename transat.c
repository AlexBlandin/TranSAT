// /* https://github.com/nothings/stb */
// #include "stretchy_buffer.h" /* requires -Wno-unused-value */

#include "s.h" /* short.hand */
#include "transat.h" /* transat header (types, common funcs, etc) */

#define FIRSTOPEN

/* Is this board trivial to solve now? */
bool satisfied() {
  u64 prev_nq = nq; /* previous queens count */
  u8 queens = 0;
  switch (bd.queens_left) {
    case 0:
    /* for all valid n-queens configs */
    for (u16 i = 0; i < N; i++) // TODO: speedup
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

/* Is this board valid / usable for further queen placement? */
bool falsified() {
  /* ALO unsatisfiable */
  if (rk.open_rows == 0 and rk.open_cols == 0)
    return true;

  /* AMO unsatisfied */
  /* Only do if using a heuristic that can give AMO unsatisfiable output */
  #if defined(FIRSTOPEN) or defined(FIRSTROW) or defined(SQUAREENUM) or defined(TAW) or defined(ANTITAW)
  for (u16 i = 0; i < N; i++) // TODO: speedup
    for (u16 j = 0; j < N; j++)
      if (rk.rows[i].placed > 1 or rk.cols[j].placed > 1 or rk.dias[i+j].placed > 1 or rk.adia[N-j+i-1].placed > 1)
        return true;
  #endif

  return false;
}

/* Pick a space any (open) space */
Slot heuristic() {
  i8 row, col; row = col = 0;
  #ifdef FIRSTOPEN
  for (u16 i = sl.row * sl.col; i < N*N; i++) {
    if (bd.state[i] == OPEN)
      return as_slot(i);
  }
  #endif
  #ifdef FIRSTROW
  #endif
  #ifdef SQUAREENUM
  #endif
  #ifdef TAW
  #endif
  #ifdef ANTITAW
  #endif
  #if not (defined(FIRSTOPEN) or defined(FIRSTROW) or defined(SQUAREENUM) or defined(TAW) or defined(ANTITAW))
  #error "You need to choose a heuristic"
  #endif
  return (Slot){row, col};
}

// clang-cl -fuse-ld=lld -Z7 -MTd transat.c -o transat.exe && remedybg dbg.rdbg

/*
  TODO: speedup!
*/

void transat() {
  bool forced = false;
  Slot queued = (Slot){0,0};
  do {
    assert(board <= N*N);
    bd.visits++;

    /* recompute the ranks */ // TODO: speedup
    zero(rk);
    for (u8 row = 0; row < N; row++) {
      for (u8 col = 0; col < N; col++) { // we can make this much faster using bitcounts of 3 bitsets
        u8 diag = row + col;
        u8 adia = N - col + row - 1;
        switch (bd.state[row*N + col]) {
          case PLACED:
          rk.rows[row].placed++;
          rk.cols[col].placed++;
          rk.dias[diag].placed++;
          rk.adia[adia].placed++;
          break;
          case FORBIDDEN:
          rk.rows[row].forbidden++;
          rk.cols[col].forbidden++;
          rk.dias[diag].forbidden++;
          rk.adia[adia].forbidden++;
          break;
          case OPEN:
          rk.rows[row].open++;
          rk.cols[col].open++;
          rk.dias[diag].open++;
          rk.adia[adia].open++;
          rk.open_rows |= 1 << row;
          rk.open_cols |= 1 << col;
          break;
        }
      }
    }

    #ifdef PRINTOUT
    if (bd.queens_left == 0) {
      printf("b[%d], ql = %d, sl = (%d, %d), loops = %"LU", nq = %"LU"\n", board, bd.queens_left, sl.row, sl.col, loops, nq);
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
      /* select branching variable (the slot/space we're focusing on) */
      if (forced) {
        sl = queued;
        forced = false;
      } else {
        sl = heuristic();
      }

      copy(sizeof(Board), boards[board], boards[board+1]);
      board++;
      bd.visits = 0;

      /* place a queen */
      bd.state[sl.row*N + sl.col] = PLACED;
      bd.queens_left--;

      /* propagate over row/column AMO */
      for (u16 i = 0; i < sl.row; i++)
        bd.state[i*N + sl.col] = FORBIDDEN;
      for (u16 i = 0; i < sl.col; i++)
        bd.state[sl.row*N + i] = FORBIDDEN;
      for (u16 i = sl.row + 1; i < N; i++)
        bd.state[i*N + sl.col] = FORBIDDEN;
      for (u16 i = sl.col + 1; i < N; i++)
        bd.state[sl.row*N + i] = FORBIDDEN;

      /* propagate over diagonal/antidiagonal AMO */ // TODO: speedup
      for (s8 i = 0; i < N; i++)
        for (s8 j = 0; j < N; j++)
          if (i + j == sl.row + sl.col and i - j == sl.row - sl.col and i != sl.row and j != sl.col)
            bd.state[i*N + j] = FORBIDDEN;

    } else {
      /* forbid a space */
      bd.state[sl.row*N + sl.col] = FORBIDDEN;

      /* ALO propagation (forced move) */
      if (rk.rows[sl.row].open - 1 == 1){
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

    loops++;
  } while(board > -1);

  #ifdef PRINTOUT
  printf("loops = %"LU", nq = %"LU" of %"LU"\n", loops, nq, solutions[N]);
  #endif
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
