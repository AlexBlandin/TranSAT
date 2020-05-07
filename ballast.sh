echo " N | FIRSTROW" && echo "-----------------"

for i in "-DBALLAST=32" "-DBALLAST=64" "-DBALLAST=128" "-DBALLAST=256" "-DBALLAST=512" #"-DBALLAST=1024"
do
  echo $i
  ./run.sh $1 FIRSTROW $i
done

echo ""
echo " N | SQUAREENUM" && echo "-----------------"
for i in "-DBALLAST=32" "-DBALLAST=64" "-DBALLAST=128" "-DBALLAST=256" "-DBALLAST=512" #"-DBALLAST=1024"
do
  echo $i
  ./run.sh $1 SQUAREENUM $i
done

echo ""
echo " N | TAW" && echo "-----------------"
for i in "-DBALLAST=32" "-DBALLAST=64" "-DBALLAST=128" "-DBALLAST=256" "-DBALLAST=512" #"-DBALLAST=1024"
do
  echo $i
  ./run.sh $1 TAW $i
done

echo ""
