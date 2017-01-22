make clean
make
WIDTH=5000
HEIGHT=5000
ITERATIONS=10
NUM_CORES=2
./imganalyze $NUM_CORES
NUM_CORES=4
./imganalyze $NUM_CORES
NUM_CORES=6
./imganalyze $NUM_CORES
echo "test finished"
