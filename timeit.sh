echo " N | "$1 && echo "-----------------"

for i in {9..14}
do
  ./run.sh $i -D$1 $2
done

echo ""
