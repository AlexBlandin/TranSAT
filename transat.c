// https://github.com/nothings/stb
#include "stretchy_buffer.h" // requires -Wno-unused-value

#include "s.h" // short.hand
#include "transat.h" // transat header (types, common funcs, etc)

bool satisfied() {


  nq += satstats();
  return;
}

bool falsified() {
  u32 open = 1;
  for (u8 i = 0; i < N; i++)
    open *= N - bd.ranks.rows[i].forbidden + bd.ranks.rows[i].placed;
  for (u8 i = 0; i < N; i++)
    open *= N - bd.ranks.cols[i].forbidden + bd.ranks.cols[i].placed;
  return open == 0;
}

// implicit AMO constraint, never puts in occupied row/col/diag/adia
Slot heuristic();

u64 transat() {
  do {
    if (not bs_in(progress, board)) {
      assert(board < N*N);
      if (satisfied() or falsified()) { board--; continue; }
      assert((board+1) < N*N);

      /* HEURISTICS */
      sl = heuristic(board); // the branching variable as chosen by our heuristics

      // copy
      copy(sizeof(Board), boards[board], boards[board+1]); // should still be faster on these small matrices
      bs_set(progress, board);
      board++;

      /* ALO Propagation */
      bs_set(boards[board].forbid, sl.row*N + sl.col);
      rk.rows[sl.row].forbidden++;
      rk.rows[sl.col].forbidden++;
      rk.rows[sl_dia].forbidden++;
      rk.rows[sl_adg].forbidden++;
    } else {
      // reenter, do other, exit
      copy(sizeof(Board), boards[board-1], boards[board]);
      bs_clear(progress, board);
      nq += placed();

      /* AMO Propagation */
      bd.queens++;
      i8 dia = sl.row + sl.col;
      i8 adg = sl.row - sl.col;
      bs_set(bd.placed, sl.row*N + sl.col);
      rk.rows[sl.row].placed = 1;
      rk.rows[sl.col].placed = 1;
      rk.rows[sl_dia].placed = 1;
      rk.rows[sl_adg].placed = 1;

      // propagate over row/column
      for (i16 i = 0; i < N; i++) {
        bs_set(bd.forbid, sl.row*N + i);
        bs_set(bd.forbid, i*N + sl.col);
      }

      // propagate over diagonal/antidiagonal
      for (i16 i = 0; i < N; i++) {
        bs_set(bd.forbid, clamp(sl.row-i,0,N-1)*N + clamp(sl.col-i,0,N-1));
        bs_set(bd.forbid, clamp(sl.row-i,0,N-1)*N + clamp(sl.col+i,0,N-1));
        bs_set(bd.forbid, clamp(sl.row+i,0,N-1)*N + clamp(sl.col+i,0,N-1));
        bs_set(bd.forbid, clamp(sl.row+i,0,N-1)*N + clamp(sl.col-i,0,N-1));
      }
    }
  } while(board);
}

int main() {
  init();

  transat();

  if (nq == solutions[N]) // addressed by N as N=0 is included
    printf("Q(%d) = %lu\n", N, nq);
  else
    printf("Uh oh, we got %lu when it should be %lu", nq, solutions[N]);

  return 0;
}
