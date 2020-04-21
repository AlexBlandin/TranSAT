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
      if (bd.state[i] == PLACED)
        queens++;
    if (queens == N)
      nq++;
    break;
    // case 1:
    // /* for all forced assignments, nq++ */
    // for (u8 i = 0; i < N; i++) {
    //   if (rk.rows[i].open == 1) {
    //     for (u8 j = 0; j < N; j++) {
    //       if ((rk.cols[j].open == 1) and
    //           (rk.dias[i+j].open == 1) and
    //           (rk.adia[N-j+i-1].open == 1) and
    //           (bd.state[i*N + j] == OPEN) and
    //           (rk.cols[j].placed <= 1) and
    //           (rk.dias[i+j].placed <= 1) and
    //           (rk.adia[N-j+i-1].placed <= 1)) {
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
  u32 open = 0;
  for (u8 i = 0; i < N; i++) {
    open += rk.rows[i].open;
    open += rk.cols[i].open;
  }
  if (open == 0)
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
    if (bd.state[i] == OPEN)
      return as_slot(i);
    // if (bs_in(bd.open, i))
    //   return as_slot(i);
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

void transat() {
  do {
    assert(board <= N*N);
    bd.visits++;

    // if (board == 0 and nq < solutions[N]) bs_clear(progress, board);
    if (satisfied() or falsified()) {
      board--;
      continue;
    }

    /* odd is placed, even is forbid */
    if (bd.visits & 1) {
    // if (bs_in(progress, board) == 0) {
      /* HEURISTICS */
      sl = heuristic(); // the branching variable as chosen by our heuristics

      /* place a queen */
      copy(sizeof(Board), boards[board], boards[board+1]);
      // bs_clearall(progress, board); // reset all forward progress
      // for (int b = (board/8)+1; b < ((N+7)/8); b++) progress[b] = 0;
      board++;

      bd.visits = 0;
      bd.queens_left--;
      bd.state[sl.row*N + sl.col] = PLACED;
      // bs_set(bd.placed, sl.row*N + sl.col);

      // /* AMO Propagation */
      // i8 dia = sl.row + sl.col;
      // i8 adg = sl.row - sl.col;
      // rk.rows[sl.row].placed++;
      // rk.cols[sl.col].placed++;
      // rk.dias[sl_dia].placed++;
      // rk.adia[sl_adg].placed++;

      // rk.rows[sl.row].open = 0;
      // rk.cols[sl.col].open = 0;
      // rk.dias[sl_dia].open = 0;
      // rk.adia[sl_adg].open = 0;

      // /* TODO: WE FORGOT THE RANKS ON THE PROPAGATED ROWS/COLS/DIAGS
      //          IT DOESN'T DECREASE OPEN ON THE OTHER ROWS?COLS/DIAGS */

      /* Easier to just recount the ranks each time? Guaranteed to be correct... */

      /* propagate over row/column */
      for (i16 i = 0; i < sl.col; i++)
        bd.state[sl.row*N + i] = FORBIDDEN;
      for (i16 i = 0; i < sl.row; i++)
        bd.state[i*N + sl.col] = FORBIDDEN;
      for (i16 i = sl.col + 1; i < N; i++)
        bd.state[sl.row*N + i] = FORBIDDEN;
      for (i16 i = sl.row + 1; i < N; i++)
        bd.state[i*N + sl.col] = FORBIDDEN;
      // for (i16 = 0; i < N; i++) {
      //   bs_set(bd.forbid, sl.row*N + i);
      //   bs_set(bd.forbid, i*N + sl.col);
      //   bs_clear(bd.open, sl.row*N + i);
      //   bs_clear(bd.open, i*N + sl.col);
      // }

      /* propagate over diagonal/antidiagonal */
      for (i16 i = 0; i < N; i++) {
        i16 d1 = clamp(sl.row-i,0,N-1)*N + clamp(sl.col-i,0,N-1);
        i16 d2 = clamp(sl.row-i,0,N-1)*N + clamp(sl.col+i,0,N-1);
        i16 d3 = clamp(sl.row+i,0,N-1)*N + clamp(sl.col+i,0,N-1);
        i16 d4 = clamp(sl.row+i,0,N-1)*N + clamp(sl.col-i,0,N-1);
        bd.state[d1] = (bd.state[d1] == PLACED) ? PLACED : FORBIDDEN;
        bd.state[d2] = (bd.state[d2] == PLACED) ? PLACED : FORBIDDEN;
        bd.state[d3] = (bd.state[d3] == PLACED) ? PLACED : FORBIDDEN;
        bd.state[d4] = (bd.state[d4] == PLACED) ? PLACED : FORBIDDEN;
        // bs_set(bd.forbid, d1);
        // bs_set(bd.forbid, d2);
        // bs_set(bd.forbid, d3);
        // bs_set(bd.forbid, d4);
      }

      /* recompute the ranks */
      memset(&rk, 0, sizeof(rk)); /* zero */
      for (u8 row = 0; row < N; row++) {
        for (u8 col = 0; col < N; col++) {
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
            break;
          }
        }
      }
    } else {
      bd.state[sl.row*N + sl.col] = FORBIDDEN;

      /* TODO: check if we can inline this to just this under all circumstances and put the full compute in the other one */
      rk.rows[sl.row].forbidden++;
      rk.cols[sl.col].forbidden++;
      rk.dias[sl_dia].forbidden++;
      rk.adia[sl_adg].forbidden++;
      rk.rows[sl.row].open--; // -= rk.rows[sl.row].open ? 1 : 0;
      rk.cols[sl.col].open--; // -= rk.cols[sl.col].open ? 1 : 0;
      rk.dias[sl_dia].open--; // -= rk.dias[sl_dia].open ? 1 : 0;
      rk.adia[sl_adg].open--; // -= rk.adia[sl_adg].open ? 1 : 0;

      /* ALO propagation (forced move) */
      if (rk.rows[sl.row].open == 1){
        for (u8 i = 0; i < N; i++) {
          if (bd.state[sl.row*N + i] == OPEN) {
            bd.state[sl.row*N + i] = PLACED;
            rk.rows[sl.row].open = 0;
          }
        }
      }
      if (rk.cols[sl.col].open == 1) {
        for (u8 i = 0; i < N; i++) {
          if (bd.state[i*N + sl.col] == OPEN) {
            bd.state[i*N + sl.col] = PLACED;
            rk.cols[sl.col].open = 0;
          }
        }
      }

      // bs_set(bd.forbid, sl.row*N + sl.col);
      // bs_clear(bd.open,sl.row*N + sl.col);


      // /* propagate */ // is this even neccessary?
      // copy(sizeof(Board), boards[board], boards[board+1]); // should still be faster on these small matrices
      // // bs_set(progress, board); // when we come back, "branch"
      // board++;
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
    printf("Q(%d) != %"LU", should be %"LU"\n", N, nq, solutions[N]);
  return 0;
}
