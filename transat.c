// /* https://github.com/nothings/stb */
// #include "stretchy_buffer.h" /* requires -Wno-unused-value */

#include "s.h" /* short.hand */
#include "transat.h" /* transat header (types, common funcs, etc) */

#define FIRSTOPEN

/* Is this board trivial to solve now? */
bool satisfied() {
  u64 _nq = nq;
  if (bd.queens_left == 1) {
    /* for all forced assignments, nq++ */
    for (u8 i = 0; i < N; i++) {
      if ((rk.rows[i].open) == 1) {
        for (u8 j = 0; j < N; j++) {
          if ((rk.cols[j].open == 1) and
              (rk.dias[i+j].open == 1) and
              (rk.adia[N-j+i-1].open == 1) and
              (bd.state[i*N + j] == OPEN) /* and doesn't violate AMO */) {
              //(bs_in(bd.open, i))) {
              nq++;
          }
        }
      }
    }
  }
  return nq != _nq;
}

/* Is this board valid / usable for further queen placement? */
bool falsified() {

  /* ALO unsatisfiable */
  u32 open = 0;
  for (u8 i = 0; i < N; i++) {
    open += rk.rows[i].open;
    open += rk.cols[i].open;
  }
  if (open == 0) return true;

  /* AMO unsatisfied */
  #if defined(FIRSTOPEN) or defined(FIRSTROW) or defined(SQUAREENUM) or defined(TAW) or defined(ANTITAW)
  /* Only do if using a heuristic that can give AMO unsatisfiable output */
  for (u8 i = 0; i < N; i++)
    if (rk.rows[i].placed > 1) return true;
  for (u8 i = 0; i < N; i++)
    if (rk.cols[i].placed > 1) return true;
  for (u8 i = 0; i < (2*N-1); i++)
    if (rk.dias[i].placed > 1) return true;
  for (u8 i = 0; i < (2*N-1); i++)
    if (rk.adia[i].placed > 1) return true;
  #endif

  return false;
}

/* Pick a space any space but NEVER AN ALREADY OCCUPIED SPACE OR ROW OR COLUMN OR DIAGONAL */
Slot heuristic() {
  i8 row, col; row = col = 0;
  #ifdef FIRSTOPEN
  for (u16 i = sl.row * sl.col; i < N*N; i++) {
    if (bd.state[i] == OPEN) return as_slot(i);
    // if (bs_in(bd.open, i)) return as_slot(i);
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
  TODO: redo `satisfied` and `unsatisfied`
*/

void transat() {
  do {
    bd.visits++;
    assert(board <= N*N);
    // if (board == 0 and nq < solutions[N]) bs_clear(progress, board);
    if (falsified() or satisfied()) { board--; continue; }
    if (bd.visits & 1) { /* odd is placed, even is forbid */
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

      /* AMO Propagation */
      i8 dia = sl.row + sl.col;
      i8 adg = sl.row - sl.col;
      rk.rows[sl.row].placed++;
      rk.cols[sl.col].placed++;
      rk.dias[sl_dia].placed++;
      rk.adia[sl_adg].placed++;

      rk.rows[sl.row].open = 0;
      rk.cols[sl.col].open = 0;
      rk.dias[sl_dia].open = 0;
      rk.adia[sl_adg].open = 0;

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
    } else {
      bd.state[sl.row*N + sl.col] = FORBIDDEN;
      // bs_set(bd.forbid, sl.row*N + sl.col);
      // bs_clear(bd.open,sl.row*N + sl.col);

      rk.rows[sl.row].forbidden++;
      rk.cols[sl.col].forbidden++;
      rk.dias[sl_dia].forbidden++;
      rk.adia[sl_adg].forbidden++;
      rk.rows[sl.row].open -= rk.rows[sl.row].open ? 1 : 0;
      rk.cols[sl.col].open -= rk.cols[sl.col].open ? 1 : 0;
      rk.dias[sl_dia].open -= rk.dias[sl_dia].open ? 1 : 0;
      rk.adia[sl_adg].open -= rk.adia[sl_adg].open ? 1 : 0;

      // /* propagate */ // is this even neccessary?
      // copy(sizeof(Board), boards[board], boards[board+1]); // should still be faster on these small matrices
      // // bs_set(progress, board); // when we come back, "branch"
      // board++;
    }
  } while(board > -1);
}

int main() {
  init();

  transat();

  if (nq == solutions[N]) /* addressed by N as N=0 is included */
    printf("Q(%d) = %"LU"\n", N, nq);
  else
    printf("Uh oh, we got %"LU" when it should be %"LU"\n", nq, solutions[N]);
  return 0;
}
