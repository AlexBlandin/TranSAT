// https://github.com/nothings/stb
#include "stretchy_buffer.h" // requires -Wno-unused-value

#include "s.h" // short.hand
#include "transat.h" // transat header (types, common funcs, etc)

u64 satstats(u16 board);

u64 falstats(u16 board);

bool satisfied() {


  nq += satstats(board);
  return;
}

bool falsified() {


  nq += falstats(board);
  return;
}

Slot heuristic(u16 board);

u64 transat() {
  do {
    if (not bs_in(progress, board)) {
      assert(board < N*N);
      if (satisfied() or falsified()) { board--; continue; }
      assert((board+1) < N*N);

      /* HEURISTICS */
      boards[board].slot = heuristic(board); // the branching variable as chosen by our heuristics

      // copy
      copy(sizeof(Board), boards[board], boards[board+1]); // should still be faster on these small matrices
      bs_set(progress, board);
      board++;

      /* FORBID SLOT */
      bs_set(boards[board].forbid, boards[board].slot.row*N + boards[board].slot.col);
      bd.ranks.rows[sl.row].forbidden++;
      bd.ranks.rows[sl.col].forbidden++;
      bd.ranks.rows[sl_dia].forbidden++;
      bd.ranks.rows[sl_adg].forbidden++;
    } else {
      // reenter, do other, exit
      copy(sizeof(Board), boards[board-1], boards[board]);
      bs_clear(progress, board);
      nq += placed();

      /* PLACED SLOT */
      i8 dia = sl.row + sl.col;
      i8 adg = sl.row - sl.col;
      bs_set(bd.placed, sl.row*N + sl.col);
      bd.ranks.rows[sl.row].placed = 1;
      bd.ranks.rows[sl.col].placed = 1;
      bd.ranks.rows[sl_dia].placed = 1;
      bd.ranks.rows[sl_adg].placed = 1;

      // propagate over row/column
      for (i16 i = 0; i < N; i++) {
        bs_set(bd.forbid, sl.row*N + i);
        bs_set(bd.forbid, i*N + sl.col);
      }

      // propagate over diagonal/antidiagonal
      for (i16 i = 0; i < N; i++) {
        bs_set(bd.forbid, clamp(sl.row-i,0,N-1)*N + clamp(sl.col-i,0,N-1));
        bs_set(bd.forbid, clamp(sl.row-i,0,N-1)*N + clamp(sl.col+i,0,N-1));
        bs_set(bd.forbid, clamp(sl.row-i,0,N-1)*N + clamp(sl.col-i,0,N-1));
        bs_set(bd.forbid, clamp(sl.row+i,0,N-1)*N + clamp(sl.col-i,0,N-1));
      }
    }
  } while(board);
}

int main() {
  init();



  u8 *sb = NULL; // stretchy buffer
  u8 d6 = randint()%6;
  sb_push(sb, d6);
  sb_push(sb, 3);
  sb[1] = N - d6;
  sb_push(sb, (u8)(randint()%100));
  sb_count(sb); // == 1
  sb_last(sb);  // lvalue of last item
  printf("Hello world! Say it again %d times louder!\n", sb[1]);
  println();

  u8 m = randint()%28;
  printf("Q(%d x %d) is %ld.\n", m, m, solutions[m]);
  sb_free(sb);

  // if (r == solutions[N]) // addressed by N as N=0 is included
  //   printf("Q(%d) = %lu\n", N, r);
  // else
  //   printf("Uh oh, we got %lu when it should be %lu", r, solutions[N]);

  return 0;
}
