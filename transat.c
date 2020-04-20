// https://github.com/nothings/stb
#include "stretchy_buffer.h" // requires -Wno-unused-value

#include "s.h" // short.hand
#include "transat.h" // transat header (types, common funcs, etc)

#define FIRSTOPEN

bool satisfied() {
  if (board > N-1) {

    // nq += satstats();
  }
  return false;
}

// ALO unsatisfiable (implicitly never AMO unsatisfiable)
bool falsified() {
  u32 open = 0;
  for (u8 i = 0; i < N; i++)
    open += N - rk.rows[i].forbidden + rk.rows[i].placed;
  for (u8 i = 0; i < N; i++)
    open += N - rk.cols[i].forbidden + rk.cols[i].placed;
  return open == 0;
}

// NEVER AMO UNSATISFIABLE aka never returns an already occupied slot
Slot heuristic() {
  i8 row, col;
  #ifdef FIRSTOPEN
  row = sl.row;
  col = sl.col;
  for (u16 i = row * col; i < N*N; i++) {
    if (!bs_in(bd.forbid, i) and !bs_in(bd.placed, i)) return as_slot(i);
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

  return (Slot){row, col};
}

void transat() {
  do {
    assert(board < N*N);
    if (bs_in(backtrack, board) or satisfied() or falsified()) { board--; continue; }
    if (bs_in(progress, board) == 0) {
      /* HEURISTICS */
      sl = heuristic(board); // the branching variable as chosen by our heuristics

      // copy
      copy(sizeof(Board), boards[board], boards[board+1]); // should still be faster on these small matrices
      bs_set(progress, board); // when we come back, "branch"
      board++;

      /* ALO Propagation */
      bs_set(boards[board].forbid, sl.row*N + sl.col);
      rk.rows[sl.row].forbidden++;
      rk.rows[sl.col].forbidden++;
      rk.rows[sl_dia].forbidden++;
      rk.rows[sl_adg].forbidden++;
    } else {
      // reenter, place instead of forbid, exit
      copy(sizeof(Board), boards[board], boards[board+1]);
      bs_set(backtrack, board); // when we come back, backtrack up
      board++;

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

    // backtrack
    board--;

  } while(board);
}

int main() {
  init();

  transat();

  if (nq == solutions[N]) // addressed by N as N=0 is included
    printf("Q(%d) = %lu\n", N, nq);
  else
    printf("Uh oh, we got %lu when it should be %lu\n", nq, solutions[N]);

  return 0;
}
