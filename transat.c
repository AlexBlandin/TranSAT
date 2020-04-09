/*
JUST BUILD && RUN:
  musl-gcc -fplan9-extensions -static transat.c -o transat && ./transat

OPTIMISE:
  musl-gcc -fplan9-extensions -static \
            -Wall -Wextra -Wno-unused-value -Wno-unused-variable \
            -Ofast -march=native -fwhole-program \
            -DNDEBUG transat.c -o transat && ./transat

*/

/* Project built against musl 1.2.0 */

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
  u64 *sb = NULL; // stretchy buffer
  u64 my_int = 10;
  sb_push(sb, my_int);
  sb_push(sb, 3);
  sb_push(sb, 0);
  sb_push(sb, randint()%100);
  sb_count(sb); // == 1
  sb_last(sb);  // lvalue of last item, == 42
  printf("Hello world! Say it again %ld times louder!\n", sb[3]);
  sb_free(sb);
  return 0;
}
