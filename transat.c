// https://github.com/nothings/stb
#include "stretchy_buffer.h" // requires -Wno-unused-value

#include "s.h" // short.hand
#include "transat.h" // transat header (types, common funcs, etc)

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

  u8 sol = randint()%28;
  printf("Q(%d x %d) is %ld.\n", sol, sol, solutions[sol]);
  sb_free(sb);

  // if (r == solutions[N]) // addressed by N as N=0 is included
  //   printf("Q(%d) = %lu\n", N, r);
  // else
  //   printf("Uh oh, we got %lu when it should be %lu", r, solutions[N]);

  return 0;
}
