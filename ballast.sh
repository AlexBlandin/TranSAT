echo " N | FIRSTROW" && echo "-----------------"

for i in 32 64 128 256 512 1024
do
  echo $i
  musl-gcc -Wall -Wextra -Wno-unused-value -Wno-unused-variable -Wimplicit-fallthrough=0 -s -static -Ofast -march=native -fwhole-program -fno-stack-protector -ffunction-sections -fdata-sections -Wl,--gc-sections -DNDEBUG -DFIRSTROW -DN=$1 -DBALLAST=$i transat.c -o transat && TIMEFORMAT='%3lU' && time taskset -c 0 ./transat
done

echo ""
echo " N | SQUAREENUM" && echo "-----------------"
for i in 32 64 128 256 512 1024
do
  echo $i
  musl-gcc -Wall -Wextra -Wno-unused-value -Wno-unused-variable -Wimplicit-fallthrough=0 -s -static -Ofast -march=native -fwhole-program -fno-stack-protector -ffunction-sections -fdata-sections -Wl,--gc-sections -DNDEBUG -DSQUAREENUM -DN=$1 -DBALLAST=$i transat.c -o transat && TIMEFORMAT='%3lU' && time taskset -c 0 ./transat
done

echo ""
echo " N | TAW" && echo "-----------------"
for i in 32 64 128 256 512 1024
do
  echo $i
  musl-gcc -Wall -Wextra -Wno-unused-value -Wno-unused-variable -Wimplicit-fallthrough=0 -s -static -Ofast -march=native -fwhole-program -fno-stack-protector -ffunction-sections -fdata-sections -Wl,--gc-sections -DNDEBUG -DTAW -DN=$1 -DBALLAST=$i transat.c -o transat && TIMEFORMAT='%3lU' && time taskset -c 0 ./transat
done

echo ""
