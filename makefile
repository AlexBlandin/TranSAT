all: 
	musl-gcc -fplan9-extensions -Wall -Wextra -Wno-unused-value -Wno-unused-variable -Wimplicit-fallthrough=0 -static -Ofast -march=native -fwhole-program -DNDEBUG transat.c -o transat
