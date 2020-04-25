musl-gcc -fplan9-extensions -Wall -Wextra -Wno-unused-value -Wno-unused-variable -Wimplicit-fallthrough=0 -static -Ofast -march=native -fwhole-program $3 -DNDEBUG -DN=$2 $1.c -o $1
