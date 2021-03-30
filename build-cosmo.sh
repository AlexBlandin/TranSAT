#!/bin/sh
gcc -g -Os -static -fno-pie -no-pie -mno-red-zone -nostdlib -nostdinc \
  -fno-omit-frame-pointer -pg -mnop-mcount -Wl,--build-id=none -Icosmo \
  $3 -DNDEBUG -DN=$2 -o $1.com.dbg $1.c -Wl,--gc-sections -fuse-ld=bfd \
  -Wl,-T,cosmo/ape.lds -include cosmo/cosmopolitan.h cosmo/crt.o cosmo/ape.o cosmo/cosmopolitan.a
objcopy -SO binary $1.com.dbg $1.com
# in theory with cosmopolitan 0.3 you can remove -Wl,--build-id=none as the build-id is stripped automatically
