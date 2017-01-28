make clean
make
NUM_CORES=1
./imganalyze $NUM_CORES 1
NUM_CORES=2
./imganalyze $NUM_CORES 0
NUM_CORES=4
./imganalyze $NUM_CORES 0
NUM_CORES=6
./imganalyze $NUM_CORES 0
NUM_CORES=8
./imganalyze $NUM_CORES 0
echo "test finished"
