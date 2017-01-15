make clean
make
WIDTH=10000
HEIGHT=10000
NUM_CORES=6
echo "running test with randomly generated image of size $WIDTH x $HEIGHT"
echo "single core"
./single $HEIGHT $WIDTH
echo "multiple core1 cores=$NUM_CORES"
./multiple1 $NUM_CORES $HEIGHT $WIDTH
echo "multiple core2 cores=$NUM_CORES"
./multiple2 $NUM_CORES $HEIGHT $WIDTH
NUM_CORES=2
echo "multiple core1 cores=$NUM_CORES"
./multiple1 $NUM_CORES $HEIGHT $WIDTH
echo "multiple core2 cores=$NUM_CORES"
./multiple2 $NUM_CORES $HEIGHT $WIDTH
echo "test finished"
