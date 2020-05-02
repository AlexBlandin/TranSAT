./build.sh transat $1 -D$2 $3 && TIMEFORMAT=' %3lU' && time taskset -c 0 ./transat
