// /* https://github.com/nothings/stb */
// #include "stretchy_buffer.h" /* requires -Wno-unused-value */

#include "transat.h" /* transat header (types & related funcs, etc, not main logic) */
#include "s.h"       /* short.hand */

/* pick a heuristic */
#if not defined(FIRSTROW) and not defined(SQUAREENUM) and not defined(TAW) and not defined(ANTITAW)
#define TAW
#endif

/* pick a space any (open) space */
static inline Slot heuristic() {
  Slot s = {0, 0, 0, 0};
#if defined(FIRSTROW) or defined(SQUAREENUM)
  /* from 0,0 indices */
  for (; bd.i < N * N; bd.i++)
    if
      lut_open(bd.i) return lut[bd.i];
#elif defined(TAW)
  float top_prod = 0, top_sum = 0;
  for (u8 row = 0; row < N; row++) {
    if
      space_left(bd.rows[row]) {
        for (u8 col = 0; col < N; col++) {
          if
            open(row, col) {
              Slot v = slot(row, col);
              WeightPair h = heuristics(v);
              float prod = h.first * h.second;
              if (prod >= top_prod) {
                float sum = h.first + h.second;
                if (sum > top_sum) {
                  top_prod = prod;
                  top_sum = sum;
                  s = v;
                }
              }
            }
        }
      }
  }
#elif defined(ANTITAW)
  float low_prod = 0, low_sum = 0;
  for (u8 row = 0; row < N; row++) {
    if
      space_left(bd.rows[row]) {
        for (u8 col = 0; col < N; col++) {
          if
            open(row, col) {
              Slot v = slot(row, col);
              WeightPair h = heuristics(v);
              float prod = h.first * h.second;
              if (prod <= low_prod) {
                float sum = h.first + h.second;
                if (sum < low_sum) {
                  low_prod = prod;
                  low_sum = sum;
                  s = v;
                }
              }
            }
        }
      }
  }
#else
#error "You need to choose a heuristic"
#endif
  return s;
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
static inline bool falsified() { return bd.falsified or bd.open == 0; }

/* for the current slot, AMO propagate */
static inline void prop_amo(Slot s) {
  /* AMO propagate over row and update ranks */
  for (u8 col = 0; space_left(bd.rows[s.row]) and col < N; col++) {
    if
      open(s.row, col) {
        set(s.row, col);
        bd.open--;
        derank(s.row, col);
      }
  }
  /* AMO propagate over column and update ranks */
  for (u8 row = 0; space_left(bd.cols[s.col]) and row < N; row++) {
    if
      open(row, s.col) {
        set(row, s.col);
        bd.open--;
        derank(row, s.col);
      }
  }

  /* AMO propagate outwards over diagonals and update ranks */
  for (u8 i = 1; (rk.dias[s.dia] or rk.adia[s.adg]) and i < N; i++) {
    u8 up = s.col - i;
    u8 down = s.col + i;
    u8 left = s.row - i;
    u8 right = s.row + i;

    /* we're using underflow to our advantage */
    if (left < N and up < N and open(left, up)) {
      set(left, up); /* rk.dias[diagonal(left, up)].open */
      bd.open--;
      derank(left, up);
    }
    if (left < N and down < N and open(left, down)) {
      set(left, down); /* rk.adia[antidiagonal(left, down)].open */
      bd.open--;
      derank(left, down);
    }
    if (right < N and up < N and open(right, up)) {
      set(right, up); /* rk.adia[antidiagonal(right, up)].open */
      bd.open--;
      derank(right, up);
    }
    if (right < N and down < N and open(right, down)) {
      set(right, down); /* rk.dias[diagonal(right, down)].open */
      bd.open--;
      derank(right, down);
    }
  }
}

/* for the current board, ALO propagate */
static inline void prop_alo() {
  static Slot queue[4] = {};
  s8 have_placed = -1; /* indices 0 to 3 in the queue or -1 (empty queue) */

  /* ALO propagate over rows/cols */
  for (u8 rowcol = 0; not falsified() and rowcol < N; rowcol++) {
    if (space_left(bd.rows[rowcol]) and n_open(bd.rows[rowcol]) == 1) {
      queue[++have_placed] = slot(rowcol, forced(bd.rows[rowcol]));
    }
    if (space_left(bd.cols[rowcol]) and n_open(bd.cols[rowcol]) == 1) {
      queue[++have_placed] = slot(forced(bd.cols[rowcol]), rowcol);
    }
  }

  if (not falsified()) {
    for (; have_placed > -1; have_placed--) {
      if
        open_sl(queue[have_placed]) {
          occupy(queue[have_placed]);
          bd.queens_left--;
          prop_amo(queue[have_placed]);
        }
      else {
        bd.falsified = true; /* the space has been closed since, meaning there is a Queen that can capture this */
      }
    }
  }
}

static inline void prop_closure() {
  u32 prev_open; /* we can tell if propagation has occured as the number of open slots will have decreased */
  /* AMO propagate */
  prop_amo(sl);

  // do {
  //   prev_open = bd.open;
  //   /* ALO propagate */
  //   prop_alo();
  // } while(not falsified() and bd.open != prev_open); /* until falsified or it stops propagating */
}

/* the TranSAT N-Queens solver */
static inline void transat() {
  while (board > -1) { /* starts at 0 */

    if (falsified() or satisfied()) {
      board--;
    } else {
      /* select branching variable (the slot/space we're focusing on) */
      sl = heuristic(); /* always chooses an open slot, so no need to worry about that */

      /* forbid a space */
      occupy(sl);
      // prop_alo();

      /* place a queen on a new board */
      board++;                         /* move along the stack */
      stack[board] = stack[board - 1]; /* clone the board */
      // copy(sizeof(Board), stack[board-1], stack[board]);
      bd.queens_left--;

      prop_closure();
    }
  }
}

int main() {
  init();
  for (u16 i = 0; i < N * N; i++) {
#if defined(FIRSTROW)
    lut[i] = slot(i / N, i % N);
#elif defined(SQUAREENUM)
    u16 r = (u16)ceil(sqrt(i + 1));
    u16 d = r * r - i - 1;
    lut[i] = (d < r) ? slot(d, r - 1) : slot(r - 1, 2 * r - d - 2);
#endif
  }
  if (N <= 1) {
    nq = 1;
  } else {
    transat();
  }

  if (nq == solutions[N]) /* addressed by N as N=0 is included */
    printf(N > 9 ? "%d |" : " %d |", N);
  else
    printf("Q(%d) gave %" LU ", should be %" LU "\n", N, nq, solutions[N]);
  return nq != solutions[N];
}
