#!/bin/bash

# Check if arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <vector_size> <repetitions>"
    exit 1
fi

N=$1
REPS=$2

# Check Python dependencies
echo "Checking Python dependencies..."
if ! python3 -c "import numpy" 2>/dev/null; then
    echo "NumPy is not installed. Installing required dependencies..."
    pip install -r ../requirements.txt
fi

# Performance warning for large datasets
if [ "$N" -gt 10000000 ]; then
    echo "Notice: Large vector size detected (N=$N)"
    echo "The pure Python implementation (C4) may take significantly longer."
    echo "This is expected as it demonstrates the performance difference"
    echo "between optimized and non-optimized implementations."
    echo "Press Enter to continue or Ctrl+C to skip this implementation..."
    read
fi

# Configure OpenBLAS threads
if [ -z "$OPENBLAS_NUM_THREADS" ]; then
    # Set to number of physical cores
    CORES=$(grep -c ^processor /proc/cpuinfo)
    export OPENBLAS_NUM_THREADS=$CORES
    echo "Set OPENBLAS_NUM_THREADS to $CORES"
fi

# Build C implementations
cd ../src
make clean
make all

echo "Running all implementations with N=$N and repetitions=$REPS"

# Run C implementations
echo -e "\nRunning C1 (Basic C)"
./c1/dp1 $N $REPS

echo -e "\nRunning C2 (Loop Unrolling)"
./c2/dp2 $N $REPS

echo -e "\nRunning C3 (BLAS)"
./c3/dp3 $N $REPS

# Run Python implementations
echo -e "\nRunning C4 (Basic Python)"
if [ "$N" -gt 10000000 ]; then
    echo "Notice: Running non-optimized implementation with large dataset"
    echo "Press Enter to continue or Ctrl+C to skip..."
    read
fi
python3 c4/dp4.py $N $REPS

echo -e "\nRunning C5 (NumPy)"
python3 c5/dp5.py $N $REPS

echo -e "\nAll implementations completed. Results are in the respective results_*.txt files." 