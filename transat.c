// /* https://github.com/nothings/stb */
// #include "stretchy_buffer.h" /* requires -Wno-unused-value */

#include "s.h" /* short.hand */
#include "transat.h" /* transat header (types, common funcs, etc) */

#define FIRSTOPEN

// TODO: is this correct?
/* Is this board trivial to solve now? */
bool satisfied() {
  u64 prev_nq = nq; /* previous queens count */
  u8 queens = 0;
  switch (bd.queens_left) {
    case 0:
    /* for all valid n-queens configs */
    for (u16 i = 0; i < N*N; i++)
      if (bd.space[i] == PLACED)
        queens++;
    if (queens == N)
      nq++;
    break;
    case 1:
    /* for all forced assignments, nq++ */
    for (u8 i = 0; i < N; i++) {
      if (rk.rows[i].open == 1) {
        for (u8 j = 0; j < N; j++) {
          if ((rk.cols[j].open == 1) and
              (rk.dias[i+j].open == 1) and
              (rk.adia[N-j+i-1].open == 1) and
              (bd.space[i*N + j] == OPEN) and
              (rk.cols[j].placed <= 1) and
              (rk.dias[i+j].placed <= 1) and
              (rk.adia[N-j+i-1].placed <= 1)) {
              //(bs_in(bd.open, i))) {
              nq++;
          }
        }
      }
    }
    break;
    default:
    break;
  }
  return nq != prev_nq;
}

/* Is this board valid / usable for further queen placement? */
bool falsified() {
  /* ALO unsatisfiable */
  if ((rk.open_rows == 0) and (rk.open_cols == 0))
    return true;

  /* AMO unsatisfied */
  /* Only do if using a heuristic that can give AMO unsatisfiable output */
  #if defined(FIRSTOPEN) or defined(FIRSTROW) or defined(SQUAREENUM) or defined(TAW) or defined(ANTITAW)
  for (u8 i = 0; i < N; i++)
    if ((rk.rows[i].placed > 1) or (rk.cols[i].placed > 1))
      return true;
  for (u8 i = 0; i < (2*N-1); i++)
    if ((rk.dias[i].placed > 1) or (rk.adia[i].placed > 1))
      return true;
  #endif

  return false;
}

/* Pick a space any (open) space */
Slot heuristic() {
  i8 row, col; row = col = 0;
  #ifdef FIRSTOPEN
  for (u16 i = sl.row * sl.col; i < N*N; i++) {
    if (bd.space[i] == OPEN)
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
  TODO: We seem to be propagating too much
*/

void transat() {
  do {
    assert(board <= N*N);
    bd.visits++;

    /* recompute the ranks */
    zero(rk);
    for (u8 row = 0; row < N; row++) {
      for (u8 col = 0; col < N; col++) {
        u8 diag = row + col;
        u8 adia = N - col + row - 1;
        switch (bd.space[row*N + col]) {
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

    // if (bd.queens_left < 2) {
      printf("b[%d], ql = %d, sl = (%d, %d), loops = %"LU".\n", board, bd.queens_left, sl.row, sl.col, loops);
      for (u16 i = 0; i < N; i++) {
        for (u16 j = 0; j < N; j++)
          printf("%d ", bd.space[i*N + j]);
        println();
      }
      println();
    // }

    if (satisfied() or falsified()) {
      // if (bd.visits & 1) {
        board--;
      // } else {
      //   bd.forced
      // }
      continue;
    }

    /* odd is placed, even is forbid */
    if (bd.visits & 1) {
      /* select branching variable (the slot/space we're focusing on) */
      sl = heuristic();

      copy(sizeof(Board), boards[board], boards[board+1]);
      board++;
      bd.visits = 0;

      /* place a queen */
      bd.space[sl.row*N + sl.col] = PLACED;
      bd.queens_left--;

      // TODO: FIX PROPOGATION
      /* propagate over row/column AMO */
      for (i16 i = 0; i < sl.col; i++)
        bd.space[sl.row*N + i] = FORBIDDEN;
      for (i16 i = 0; i < sl.row; i++)
        bd.space[i*N + sl.col] = FORBIDDEN;
      for (i16 i = sl.col + 1; i < N; i++)
        bd.space[sl.row*N + i] = FORBIDDEN;
      for (i16 i = sl.row + 1; i < N; i++)
        bd.space[i*N + sl.col] = FORBIDDEN;

      /* propagate over diagonal/antidiagonal AMO */ // TODO: This is probably wrong
      for (u16 i = 0; i < N; i++) {
        u16 d1 = clamp(sl.row-i,0,N-1)*N + clamp(sl.col-i,0,N-1);
        u16 d2 = clamp(sl.row-i,0,N-1)*N + clamp(sl.col+i,0,N-1);
        u16 d3 = clamp(sl.row+i,0,N-1)*N + clamp(sl.col+i,0,N-1);
        u16 d4 = clamp(sl.row+i,0,N-1)*N + clamp(sl.col-i,0,N-1);
        bd.space[d1] = (bd.space[d1] == PLACED) ? PLACED : FORBIDDEN;
        bd.space[d2] = (bd.space[d2] == PLACED) ? PLACED : FORBIDDEN;
        bd.space[d3] = (bd.space[d3] == PLACED) ? PLACED : FORBIDDEN;
        bd.space[d4] = (bd.space[d4] == PLACED) ? PLACED : FORBIDDEN;
      }
    } else {
      /* forbid a space */
      bd.space[sl.row*N + sl.col] = FORBIDDEN;

      // copy(sizeof(Board), boards[board], boards[board+1]);
      // board++;
      // bd.visits = 0;

      // /* ALO propagation (forced move) */
      // if (rk.rows[sl.row].open - 1 == 1){
      //   for (u8 i = 0; i < N; i++) {
      //     if (bd.space[sl.row*N + i] == OPEN) {
      //       bd.space[sl.row*N + i] = PLACED;
      //       rk.rows[sl.row].open = 0;
      //     }
      //   }
      // }
      // if (rk.cols[sl.col].open - 1 == 1) {
      //   for (u8 i = 0; i < N; i++) {
      //     if (bd.space[i*N + sl.col] == OPEN) {
      //       bd.space[i*N + sl.col] = PLACED;
      //       rk.cols[sl.col].open = 0;
      //     }
      //   }
      // }

    }

    loops++;
  } while(board > -1);
}

int main() {
  init();

  transat();

  if (nq == solutions[N]) /* addressed by N as N=0 is included */
    printf("Q(%d) = %"LU"\n", N, nq);
  else
    printf("Q(%d) gave %"LU", should be %"LU"\n", N, nq, solutions[N]);
  return 0;
}
