// https://github.com/nothings/stb
#include "stretchy_buffer.h" // requires -Wno-unused-value

#include "s.h" // short.hand

#define N 32

// n MUST BE A CONSTANT
#define queen(n) if (n < N) queens.q##n

// Aligned to 8byte boundary
typedef union _queens {
  struct {
    u8 q0, q1, q2, q3, q4, q5, q6, q7;
    #if N > 8
    u8 q8, q9, q10, q11, q12, q13, q14, q15;
    #endif
    #if N > 16
    u8 q16, q17, q18, q19, q20, q21, q22, q23;
    #endif
    #if N > 24
    u8 q24, q25, q26, q27, q28, q29, q30, q31;
    #endif
  };
  u8 q[N];
} Queens;

static Queens queens;
static u32 queens_mask = 0; // which queens are placed (bitmask)
static u8 board[N*N][N*N/8] = {}; // open/forbidden ALCS boards


static u64 solutions[] = {1, 1, 0, 0, 2, 10, 4, 40, 92, 352,
                          724, 2680, 14200, 73712, 365596,
                          2279184, 14772512, 95815104, 666090624,
                          4968057848, 39029188884, 314666222712,
                          2691008701644, 24233937684440,
                          227514171973736, 2207893435808352,
                          22317699616364044, 234907967154122528};

void init() {
  seed_rng();
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

  u8 sol = randint()%28;
  printf("Q(%d x %d) is %ld.\n", sol, sol, solutions[sol]);
  sb_free(sb);

  return 0;
}
