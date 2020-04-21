clear && musl-gcc -fplan9-extensions -Wall -Wextra -Wno-unused-value -Wno-unused-variable -Wimplicit-fallthrough=0 -static -Ofast -march=native $2 $1.c -o $1 && time ./$1
