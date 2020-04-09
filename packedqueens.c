#include "s.h"

#define N 32

#define QUEENS Queen q0, q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, q11, q12, q13, q14, q15, q16, q17, q18, q19, q20, q21, q22, q23, q24, q25, q26, q27, q28, q29, q30, q31

#define qs(n) if ((n) < N) qs.q##n

// gets to use a bit on each for bounds checking (if no #if) and a bit for open/close
typedef union _queen {
  struct {
    u8 pos : 5;
    u8 open : 1;
    u8 bounds : 1;
  };
  u8 b;
} Queen;

// gets to use a bit on each for bounds checking (if not compiletime) and a bit for open/close
typedef union _queens {
  struct {
    QUEENS;
  };
  Queen q[N];
} Queens;

int main() {
  seed_rng();
  Queens qs;

  for (u64 i = (u64)1 << 32; i--;) {
    u64 x = randint();
    qs(0).b = x++;
    qs(1).b = x++;
    qs(2).b = x++;
    qs(3).b = x++;
    qs(4).b = x++;
    qs(5).b = x++;
    qs(6).b = x++;
    qs(7).b = x++;
    qs(8).b = x++;
    qs(9).b = x++;
    qs(10).b = x++;
    qs(11).b = x++;
    qs(12).b = x++;
    qs(13).b = x++;
    qs(14).b = x++;
    qs(15).b = x++;
    qs(16).b = x++;
    qs(17).b = x++;
    qs(18).b = x++;
    qs(19).b = x++;
    qs(20).b = x++;
    qs(21).b = x++;
    qs(22).b = x++;
    qs(23).b = x++;
    qs(24).b = x++;
    qs(25).b = x++;
    qs(26).b = x++;
    qs(27).b = x++;
    qs(28).b = x++;
    qs(29).b = x++;
    qs(30).b = x++;
    qs(31).b = x++;
  }

  printf("%d\n", qs.q[N - 1].b);

  return 0;
}
