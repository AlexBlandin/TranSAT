echo " N | Time" && echo "-----------------"

for i in {1..21}
do
  ./run.sh $i $1
done
