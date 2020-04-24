echo " N | Time" && echo "-----------------"

for i in {8..21}
do
  ./run.sh $i $1
done
