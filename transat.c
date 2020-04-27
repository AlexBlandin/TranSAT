// /* https://github.com/nothings/stb */
// #include "stretchy_buffer.h" /* requires -Wno-unused-value */

#include "s.h" /* short.hand */
#include "transat.h" /* transat header (types & related funcs, etc, not main logic) */

/* pick a heuristic */
#if not defined(FIRSTROW) or not defined(SQUAREENUM) or not defined(TAW) or not defined(ANTITAW)
#define SQUAREENUM
#endif

/* pick a space any (open) space */
static inline Slot heuristic() {
  #if defined(FIRSTROW) or defined(SQUAREENUM)
  /* from 0,0 indices */
  for (; bd.i < N*N; bd.i++)
    if lut_open(bd.i)
      return lut[bd.i];
  #elif defined(TAW)
  /* TODO: */
  
  #elif defined(ANTITAW)
  /* TODO: */
  
  #else
  #error "You need to choose a heuristic"
  #endif
  return (Slot) {};
}

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
static inline bool falsified() {
  return bd.open == 0; /* ALO unsatisfiable, as AMO sat is guaranteed by heuristic generation */
}

/* the TranSAT N-Queens solver */
static inline void transat() {
  while(board > -1) { /* starts at 0 */
    bd.visits++;

    if (falsified() or satisfied()) {
      board--;
    } else {

      /* odd is placed, even is forbid */
      if (bd.visits & 1) {
        /* select branching variable (the slot/space we're focusing on) */
        sl = heuristic(); /* always chooses an open slot, so no need to worry about that */
        
        /* place a queen */
        copy_board();
        set(sl.row, sl.col);
        bd.queens_left--;
        bd.open--;
        derank_sl();
        
        /* propagate over row and update ranks */
        for (u8 col = 0; (bd.rows[sl.row] != full_row or bd.cols[sl.col] != full_row) and col < N; col++) {
          if open(sl.row, col) {
            set(sl.row, col);
            bd.open--;
            derank(sl.row, col);
            // cf_col(col);
          }
        }
        /* propagate over column and update ranks */
        for (u8 row = 0; bd.cols[sl.col] != full_row and row < N; row++) {
          if open(row, sl.col) {
            set(row, sl.col);
            bd.open--;
            derank(row, sl.col);
            // cf_row(row);
          }
        }
        // clear_full(sl.row, sl.col);

        /* propagate outwards over diagonals and update ranks */
        for (u8 i = 1; (rk.dias[sl.dia] or rk.adia[sl.adg]) and i < N; i++) {
          u8 up = sl.col-i; u8 down = sl.col+i;
          u8 left = sl.row-i; u8 right = sl.row+i;
          
          /* we're using underflow to our advantage */
          if (left < N and up < N and open(left, up)) {
            set(left, up); /* rk.dias[diagonal(left, up)].open */
            bd.open--;
            derank(left, up);
            // clear_full(left, up);
          }
          if (left < N and down < N and open(left, down)) {
            set(left, down); /* rk.adia[antidiagonal(left, down)].open */
            bd.open--;
            derank(left, down);
            // clear_full(left, down);
          }
          if (right < N and up < N and open(right, up)) {
            set(right, up); /* rk.adia[antidiagonal(right, up)].open */
            bd.open--;
            derank(right, up);
            // clear_full(right, up);
          }
          if (right < N and down < N and open(right, down)) {
            set(right, down); /* rk.dias[diagonal(right, down)].open */
            bd.open--;
            derank(right, down);
            // clear_full(right, down);
          }
        }
      } else {
        /* forbid a space */
        set(sl.row, sl.col);
        bd.open--;
        derank_sl();
        // clear_full(sl.row, sl.col);
      }
    }
  }
}

int main() {
  init();
  for (u16 i = 0; i < N*N; i++) {
  #if defined(FIRSTROW)
    lut[i] = slot(i/N, i%N);
  #elif defined(SQUAREENUM)
    u16 r = (u16)ceil(sqrt(i+1));
    u16 d = r*r - i - 1;
    lut[i] = (d < r) ? slot(d, r - 1) : slot(r - 1, 2*r - d - 2);
  #endif
  }

  transat();

  if (nq == solutions[N]) /* addressed by N as N=0 is included */
    printf(N > 9 ? "%d |" : " %d |", N);
  else
    printf("Q(%d) gave %"LU", should be %"LU"\n", N, nq, solutions[N]);
  return nq != solutions[N];
}
