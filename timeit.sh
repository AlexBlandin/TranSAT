echo " N | FIRSTROW" && echo "-----------------"

for i in {9..13}
do
  ./run.sh $i FIRSTROW $1
done

echo ""
echo " N | SQUAREENUM" && echo "-----------------"
for i in {9..13}
do
  ./run.sh $i SQUAREENUM $1
done

echo ""
echo " N | TAW" && echo "-----------------"
for i in {9..13}
do
  ./run.sh $i TAW $1
done

echo ""
