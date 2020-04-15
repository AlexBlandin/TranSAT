// https://github.com/nothings/stb
#include "stretchy_buffer.h" // requires -Wno-unused-value

#include "s.h" // short.hand
#include "transat.h" // transat header (types, common funcs, etc)


bool satisfied(u16 board);

bool falsified(u16 board);

u64 satstats(u16 board);

u64 falstats(u16 board);

Slot next_open(u16 board);

u64 forbid(u16 board, Slot slot) {
  boards[board][slot.row*N+slot.col] |= 1 << (slot.col>>3);
  return backtrack(board);
}

u64 placed(u16 board, Slot slot) {
  u8 diag = slot.row + slot.col; // slot.adg for antidiagonal
  queens.q[slot.row] = slot.col;
  queens_mask[queens_count++] = 1 << slot.row;
  for (int i = 0; i < N; i++)
    boards[board][slot.row*N + i] = boards[board][i*N + slot.col] = 1;
  return backtrack(board);
}

u64 backtrack(u16 board) {
  if (satisfied(board)) return satstats(board);
  if (falsified(board)) return falstats(board);
  u16 branch = board+1; // for our clauseset copy
  progress[board] = 0; // cleanup on entry

  Slot slot = next_open(board); // the branching variable as chosen by our heuristics
  slots[board] = slot; // keep a note

  // do one
  copy(N*N/8, boards[board], boards[branch]); // should still be faster on these small matrices
  nq += forbid(branch, slot);

  // reenter, do other
  copy(N*N/8, boards[board], boards[branch]); // should still be faster on these small matrices
  nq += placed(branch, slot);

  // now reset `slot` and get the next one

  // return
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
