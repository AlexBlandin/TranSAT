// https://github.com/nothings/stb
#include "stretchy_buffer.h" // requires -Wno-unused-value

#include "s.h" // short.hand
#include "transat.h" // transat header (types, common funcs, etc)

#define FIRSTOPEN

bool satisfied() {
  u64 _nq = nq;
  if (bd.queens == N-1) {
    // for all forced assignments, nq++
    for (u8 i = 0; i < N; i++) {
      if ((rk.rows[i].open) == 1) {
        for (u8 j = 0; j < N; j++) {
          if ((rk.cols[j].open == 1) and
              (rk.dias[i+j].open == 1) and
              (rk.adia[N-j+i-1].open == 1) and
              (bd.state[i*N + j] == OPEN)) {
              //(bs_in(bd.open, i))) {
              nq++;
          }
        }
      }
    }
  }
  return nq != _nq;
}

// ALO unsatisfiable (implicitly never AMO unsatisfiable)
bool falsified() {
  u32 open = 0;
  for (u8 i = 0; i < N; i++)
    open += rk.rows[i].open;
  for (u8 i = 0; i < N; i++)
    open += rk.cols[i].open;
  return open == 0;
}

// NEVER AMO UNSATISFIABLE aka never returns an already occupied slot
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
  #if !defined(FIRSTOPEN) and !defined(FIRSTROW) and !defined(SQUAREENUM) and !defined(TAW) and !defined(ANTITAW)
  #error "You need to choose a heuristic"
  #endif
  return (Slot){row, col};
}

void transat() {
  do {
    if (board == 0 and nq < solutions[N]) bs_clear(progress, board);
    if (satisfied() or falsified() or bs_in(backtrack, board) or board >= N*N) { board--; continue; }
    if (bs_in(progress, board) == 0) {
      /* HEURISTICS */
      sl = heuristic(); // the branching variable as chosen by our heuristics

      // copy
      copy(sizeof(Board), boards[board], boards[board+1]); // should still be faster on these small matrices
      bs_set(progress, board); // when we come back, "branch"
      board++;

      bd.state[sl.row*N + sl.col] = FORBIDDEN;
      // bs_set(bd.forbid, sl.row*N + sl.col);
      // bs_clear(bd.open,sl.row*N + sl.col);

      /* ALO Propagation */
      rk.rows[sl.row].forbidden++;
      rk.cols[sl.col].forbidden++;
      rk.dias[sl_dia].forbidden++;
      rk.adia[sl_adg].forbidden++;
      rk.rows[sl.row].open--;
      rk.cols[sl.col].open--;
      rk.dias[sl_dia].open--;
      rk.adia[sl_adg].open--;
    } else {
      // reenter, place instead of forbid, exit
      copy(sizeof(Board), boards[board], boards[board+1]);
      bs_set(backtrack, board); // when we come back, backtrack up
      board++;

      bd.queens++;
      bd.state[sl.row*N + sl.col] = PLACED;
      //bs_set(bd.placed, sl.row*N + sl.col);

      /* AMO Propagation */
      i8 dia = sl.row + sl.col;
      i8 adg = sl.row - sl.col;
      rk.rows[sl.row].placed = 1;
      rk.cols[sl.col].placed = 1;
      rk.dias[sl_dia].placed = 1;
      rk.adia[sl_adg].placed = 1;

      rk.rows[sl.row].open = 0;
      rk.cols[sl.col].open = 0;
      rk.dias[sl_dia].open = 0;
      rk.adia[sl_adg].open = 0;

      // propagate over row/column
      for (i16 i = 0; i < N; i++) {
        bd.state[sl.row*N + i] = FORBIDDEN;
        bd.state[i*N + sl.col] = FORBIDDEN;
        // bs_set(bd.forbid, sl.row*N + i);
        // bs_set(bd.forbid, i*N + sl.col);
        // bs_clear(bd.open, sl.row*N + i);
        // bs_clear(bd.open, i*N + sl.col);
      }

      // propagate over diagonal/antidiagonal
      for (i16 i = 0; i < N; i++) {
        bd.state[clamp(sl.row-i,0,N-1)*N + clamp(sl.col-i,0,N-1)] = (bd.state[clamp(sl.row-i,0,N-1)*N + clamp(sl.col-i,0,N-1)] == PLACED) ? PLACED : FORBIDDEN;
        bd.state[clamp(sl.row-i,0,N-1)*N + clamp(sl.col+i,0,N-1)] = (bd.state[clamp(sl.row-i,0,N-1)*N + clamp(sl.col+i,0,N-1)] == PLACED) ? PLACED : FORBIDDEN;
        bd.state[clamp(sl.row+i,0,N-1)*N + clamp(sl.col+i,0,N-1)] = (bd.state[clamp(sl.row+i,0,N-1)*N + clamp(sl.col+i,0,N-1)] == PLACED) ? PLACED : FORBIDDEN;
        bd.state[clamp(sl.row+i,0,N-1)*N + clamp(sl.col-i,0,N-1)] = (bd.state[clamp(sl.row+i,0,N-1)*N + clamp(sl.col-i,0,N-1)] == PLACED) ? PLACED : FORBIDDEN;
        // bs_set(bd.forbid, clamp(sl.row-i,0,N-1)*N + clamp(sl.col-i,0,N-1));
        // bs_set(bd.forbid, clamp(sl.row-i,0,N-1)*N + clamp(sl.col+i,0,N-1));
        // bs_set(bd.forbid, clamp(sl.row+i,0,N-1)*N + clamp(sl.col+i,0,N-1));
        // bs_set(bd.forbid, clamp(sl.row+i,0,N-1)*N + clamp(sl.col-i,0,N-1));
      }
    }
  } while(board);
}

int main() {
  init();

  transat();

  if (nq == solutions[N]) // addressed by N as N=0 is included
    printf("Q(%d) = %llu\n", N, nq);
  else
    printf("Uh oh, we got %llu when it should be %llu\n", nq, solutions[N]);
  return 0;
}
