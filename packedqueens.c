#include "s.h"

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

void init() {
  seed_rng();
}

int main() {
  init();

  for (u64 i = (u64)1 << 32; i--;) {
    u64 x = randint();
    queen(0) = x++;
    queen(1) = x++;
    queen(2) = x++;
    queen(3) = x++;
    queen(4) = x++;
    queen(5) = x++;
    queen(6) = x++;
    queen(7) = x++;
    #if N > 8
    queen(8) = x++;
    queen(9) = x++;
    queen(10) = x++;
    queen(11) = x++;
    queen(12) = x++;
    queen(13) = x++;
    queen(14) = x++;
    queen(15) = x++;
    #endif
    #if N > 16
    queen(16) = x++;
    queen(17) = x++;
    queen(18) = x++;
    queen(19) = x++;
    queen(20) = x++;
    queen(21) = x++;
    queen(22) = x++;
    queen(23) = x++;
    #endif
    #if N > 24
    queen(24) = x++;
    queen(25) = x++;
    queen(26) = x++;
    queen(27) = x++;
    queen(28) = x++;
    queen(29) = x++;
    queen(30) = x++;
    queen(31) = x++;
    #endif
  }

  printf("%d\n", queens.q[N - 1]);

  return 0;
}
