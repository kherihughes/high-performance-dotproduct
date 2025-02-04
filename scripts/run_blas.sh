#!/bin/bash

# Exit on error
set -e

# Function to print section headers
print_header() {
    echo -e "\n=== $1 ==="
}

# Set OpenBLAS environment variables
export OPENBLAS_NUM_THREADS=8  # Match physical cores
export OPENBLAS_DYNAMIC=0      # Disable dynamic threading
export OPENBLAS_VERBOSE=1      # Show threading info

# Navigate to source directory
cd ../src || { echo "Error: Cannot find source directory"; exit 1; }

print_header "Building Implementation"
make clean
make c3/dp3 || { echo "Error: Build failed"; exit 1; }

print_header "OpenBLAS Configuration"
echo "OPENBLAS_NUM_THREADS=$OPENBLAS_NUM_THREADS"
echo "OPENBLAS_DYNAMIC=$OPENBLAS_DYNAMIC"
echo "OPENBLAS_VERBOSE=$OPENBLAS_VERBOSE"

# Check if executable exists
if [ ! -f "c3/dp3" ]; then
    echo "Error: Executable not found"
    exit 1
fi

print_header "Running Small Test (N=1,000,000)"
./c3/dp3 1000000 1000 || { echo "Error: Small test failed"; exit 1; }

print_header "Running Large Test (N=300,000,000)"
./c3/dp3 300000000 20 || { echo "Error: Large test failed"; exit 1; }

print_header "All Tests Complete"
echo "Results written to:"
echo "- results_c3_1000000.txt"
echo "- results_c3_300000000.txt" 