// https://github.com/nothings/stb
#include "stretchy_buffer.h" // requires -Wno-unused-value

// #define NDEBUG
#include "s.h" // short.hand

static u64 solutions[] = {1, 1, 0, 0, 2, 10, 4, 40, 92, 352,
                          724, 2680, 14200, 73712, 365596,
                          2279184, 14772512, 95815104, 666090624,
                          4968057848, 39029188884, 314666222712,
                          2691008701644, 24233937684440,
                          227514171973736, 2207893435808352,
                          22317699616364044, 234907967154122528};

int main() {
  seed_rng();
  u8 *sb = NULL; // stretchy buffer
  u8 d6 = (u8)(randint()%6);
  sb_push(sb, d6);
  sb_push(sb, 3);
  sb_push(sb, 0);
  sb_push(sb, (u8)(randint()%100));
  sb_count(sb); // == 1
  sb_last(sb);  // lvalue of last item, == 42
  printf("Hello world! Say it again %d times louder!\n", sb[3]);
  u8 sol = randint()%28;
  printf("Q(%d x %d) is %ld.\n", sol, sol, solutions[sol]);
  sb_free(sb);
  return 0;
}
