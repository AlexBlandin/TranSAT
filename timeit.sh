echo " N | FIRSTROW" && echo "-----------------"

for i in {9..14}
do
  ./run.sh $i FIRSTROW $1
done

echo " N | SQUAREENUM" && echo "-----------------"
for i in {9..14}
do
  ./run.sh $i SQUAREENUM $1
done

echo " N | TAW" && echo "-----------------"
for i in {9..14}
do
  ./run.sh $i TAW $1
done

echo ""
