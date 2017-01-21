make clean
make
WIDTH=500
HEIGHT=500
NUM_CORES=6
ITERATIONS=100
echo "running test with randomly generated image of size $WIDTH x $HEIGHT"
echo "single core"
./imganalyze $NUM_CORES $ITERATIONS 1 $HEIGHT $WIDTH
NUM_CORES=2
echo "multiple core1 cores=$NUM_CORES"
./imganalyze $NUM_CORES $ITERATIONS 0 $HEIGHT $WIDTH
NUM_CORES=4
echo "multiple core1 cores=$NUM_CORES"
./imganalyze $NUM_CORES $ITERATIONS 0 $HEIGHT $WIDTH
echo "test finished"
