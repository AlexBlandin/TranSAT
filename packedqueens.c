#include "s.h"

#include "transat.h"

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
    queen(8) = x++;
    queen(9) = x++;
    queen(10) = x++;
    queen(11) = x++;
    queen(12) = x++;
    queen(13) = x++;
    queen(14) = x++;
    queen(15) = x++;
    queen(16) = x++;
    queen(17) = x++;
    queen(18) = x++;
    queen(19) = x++;
    queen(20) = x++;
    queen(21) = x++;
    queen(22) = x++;
    queen(23) = x++;
  }

  printf("%d\n", queens.q[N - 1]);

  return 0;
}
