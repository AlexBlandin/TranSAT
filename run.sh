musl-gcc -fplan9-extensions -Wall -Wextra -Wno-unused-value -Wno-unused-variable -Wimplicit-fallthrough=0 -static -Ofast -march=native -fwhole-program $2 -DNDEBUG -DN=$1 transat.c -o transat && TIMEFORMAT=' %3lU' && time taskset -c 0 ./transat
