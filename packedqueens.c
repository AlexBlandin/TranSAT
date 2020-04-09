#include "s.h"

#define N 32

#define qs(n) if((n) < N) qs.q ## n

// gets to use a bit on each for bounds checking (if no #if) and a bit for open/close
typedef union _queen {
  struct {
    u8 pos: 5;
    u8 open:1;
    u8 bounds:1;
  };
  u8 b;
} queen;

// gets to use a bit on each for bounds checking (if no #if) and a bit for open/close
typedef union _queens {
  struct {
    queen q0, q1, q2, q3, q4, q5, q6, q7;
    queen q8, q9, q10, q11, q12, q13, q14, q15;
    queen q16, q17, q18, q19, q20, q21, q22, q23;
    queen q24, q25, q26, q27, q28, q29, q30, q31;
  };
  queen q[N];
} Queens;

int main() {
  seed_rng();
  Queens qs;

  for (u64 i = (u64)1 << 32; i--;) {
    u64 x = randint();
    for (u8 j = N; j--;)
      qs.q[j].b = x++;
  }

  printf("%d\n", qs.q[N - 1].b);

  return 0;
}
