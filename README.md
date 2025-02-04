# High Performance Dot Product Implementation

This repository contains multiple implementations of dot product operations, focusing on performance optimization using different approaches:

## Implementations

1. **C1 (Basic C Implementation)**
   - Basic C implementation of dot product
   - Serves as a baseline for performance comparison

2. **C2 (Optimized C Implementation)**
   - Loop unrolling and vectorization optimizations
   - Enhanced memory access patterns
   - Aligned memory allocation

3. **C3 (BLAS Implementation)**
   - Uses OpenBLAS for optimal performance
   - Hardware-specific optimizations
   - Multi-threaded execution support

4. **C4 (Python Implementation)**
   - Pure Python implementation
   - Demonstrates baseline Python performance

5. **C5 (NumPy Implementation)**
   - Uses NumPy's optimized operations
   - Python with native performance

## Performance Metrics

Each implementation is measured for:
- Execution time
- Memory bandwidth (GB/s)
- Computational performance (FLOPS)
- Result accuracy

## Requirements

- GCC compiler with optimization support
- OpenBLAS library
- Python 3.x with NumPy (for C4 and C5)

## Building

```bash
cd src
make clean && make all
```

## Running Tests

Small dataset (1M elements):
```bash
cd scripts
./run_blas.sh
```

Large dataset (300M elements):
```bash
./run_blas.sh
```

## Results

Performance results are automatically saved to text files in the format:
`results_[implementation]_[size].txt`

## License

MIT License 