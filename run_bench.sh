make > /dev/null
export OMP_NUM_THREADS=6
WIDTH=6400
HEIGHT=5120
echo "single core"
/usr/bin/time -f "Elapsed Time = %E" ./single $HEIGHT $WIDTH > /dev/null
echo "multiple core1 cores=$OMP_NUM_THREADS"
/usr/bin/time -f "Elapsed Time = %E" ./multiple1 $HEIGHT $WIDTH > /dev/null
echo "multiple core2 cores=$OMP_NUM_THREADS"
/usr/bin/time -f "Elapsed Time = %E" ./multiple2 $HEIGHT $WIDTH > /dev/null
echo "test finished"
