./build.sh transat $1 $2 && TIMEFORMAT=' %3lU' && time taskset -c 0 ./transat
