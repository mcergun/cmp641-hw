make > /dev/null
WIDTH=10000
HEIGHT=10000
NUM_CORES=6
echo "running test with randomly generated image of size $WIDTH x $HEIGHT"
echo "single core"
/usr/bin/time -f "Elapsed Time = %E" ./single $HEIGHT $WIDTH > /dev/null
echo "multiple core1 cores=$NUM_CORES"
/usr/bin/time -f "Elapsed Time = %E" ./multiple1 $NUM_CORES $HEIGHT $WIDTH > /dev/null
echo "multiple core2 cores=$NUM_CORES"
/usr/bin/time -f "Elapsed Time = %E" ./multiple2 $NUM_CORES $HEIGHT $WIDTH > /dev/null
NUM_CORES=2
echo "multiple core1 cores=$NUM_CORES"
/usr/bin/time -f "Elapsed Time = %E" ./multiple1 $NUM_CORES $HEIGHT $WIDTH > /dev/null
echo "multiple core2 cores=$NUM_CORES"
/usr/bin/time -f "Elapsed Time = %E" ./multiple2 $NUM_CORES $HEIGHT $WIDTH > /dev/null
echo "test finished"
