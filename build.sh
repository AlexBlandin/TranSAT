clear && musl-gcc -fplan9-extensions -static -Wall -Wextra -Wno-unused-value -Wno-unused-variable -Wimplicit-fallthrough=0 -Ofast -march=native -fwhole-program -funroll-loops $2 -DNDEBUG $1.c -o $1 && ./$1
