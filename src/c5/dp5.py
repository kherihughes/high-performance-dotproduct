import numpy as np
import time
import sys
import signal

def signal_handler(signum, frame):
    print("\nInterrupted by user. Exiting gracefully...")
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

def np_dot(N, A, B):
    print("\rComputing dot product...", end="", flush=True)
    result = np.dot(A, B)
    print("\rDot product completed!")
    return result

def run_np_dot(N, reps, filename):
    print(f"\nRunning NumPy dot product with N={N:,} elements")
    A = np.ones(N, dtype=np.float32)
    B = np.ones(N, dtype=np.float32)

    total_time = 0.0
    
    with open(filename, "w") as file:
        for r in range(reps):
            print(f"\nRepetition {r + 1}/{reps}")
            start = time.monotonic()
            result = np_dot(N, A, B)
            end = time.monotonic()

            elapsed = end - start
            total_time += elapsed

            # Calculate performance metrics
            gb_per_sec = (2.0 * N * 4) / (elapsed * 1e9)  # 2 arrays read, 4 bytes per float
            flops = (2.0 * N) / elapsed  # 1 multiply + 1 add per iteration

            print(f"Time: {elapsed:.6f} sec, Bandwidth: {gb_per_sec:.2f} GB/s, FLOPS: {flops:.2e}")

            file.write(f"Rep {r + 1}: Time = {elapsed:.6f} sec, Result = {result:.6f}, "
                      f"Bandwidth = {gb_per_sec:.2f} GB/s, FLOPS = {flops:.2e}\n")

        avg_time = total_time / reps
        avg_gb_per_sec = (2.0 * N * 4) / (avg_time * 1e9)
        avg_flops = (2.0 * N) / avg_time

        file.write(f"\nAverages over {reps} repetitions:\n")
        file.write(f"Time = {avg_time:.6f} sec\n")
        file.write(f"Bandwidth = {avg_gb_per_sec:.2f} GB/s\n")
        file.write(f"FLOPS = {avg_flops:.2e}\n")

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <vector_size> <repetitions>")
        sys.exit(1)

    N = int(sys.argv[1])
    reps = int(sys.argv[2])
    
    filename = f"results_c5_{N}.txt"
    run_np_dot(N, reps, filename)

if __name__ == "__main__":
    main() 