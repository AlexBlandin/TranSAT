musl-gcc -fplan9-extensions -Wall -Wextra -Wno-unused-value -Wno-unused-variable -Wimplicit-fallthrough=0 -static -Ofast -march=native -fwhole-program $2 -DNDEBUG $1.c -o $1 && time ./$1
