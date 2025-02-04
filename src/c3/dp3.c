#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cblas.h>

float bdp(long N, float *pA, float *pB) {
    // Using cblas_sdot for single precision dot product
    return cblas_sdot(N, pA, 1, pB, 1);
}

void run_bdp(long N, int reps, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s for writing.\n", filename);
        return;
    }

    // Align memory for better performance
    float *pA = (float *)aligned_alloc(64, N * sizeof(float));
    float *pB = (float *)aligned_alloc(64, N * sizeof(float));
    if (!pA || !pB) {
        fprintf(stderr, "Memory allocation failed. Required memory: %ld bytes\n", 2L * N * sizeof(float));
        if (pA) free(pA);
        if (pB) free(pB);
        fclose(file);
        return;
    }

    // Initialize arrays and print progress for large arrays
    if (N > 100000000) printf("Initializing arrays...\n");
    for (long i = 0; i < N; i++) {
        pA[i] = 1.0f;
        pB[i] = 1.0f;
    }
    if (N > 100000000) printf("Initialization complete. Starting computations...\n");

    struct timespec start, end;
    double total_time = 0.0;

    for (int r = 0; r < reps; r++) {
        if (N > 100000000 && (r == 0 || r == reps-1 || r % 5 == 0)) {
            printf("Running repetition %d/%d...\n", r+1, reps);
        }

        clock_gettime(CLOCK_MONOTONIC, &start);
        float result = bdp(N, pA, pB);
        clock_gettime(CLOCK_MONOTONIC, &end);

        double elapsed = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1e9;
        total_time += elapsed;

        // Calculate performance metrics
        double gb_per_sec = (2.0 * N * sizeof(float)) / (elapsed * 1e9);
        double flops = (2.0 * N) / elapsed;
        
        fprintf(file, "Rep %d: Time = %.6f sec, Result = %.6f, Bandwidth = %.2f GB/s, FLOPS = %.2e\n",
                r + 1, elapsed, result, gb_per_sec, flops);
        
        // Flush file buffer periodically for long runs
        if (N > 100000000 && (r % 10 == 0)) fflush(file);
    }

    double avg_time = total_time / reps;
    double avg_gb_per_sec = (2.0 * N * sizeof(float)) / (avg_time * 1e9);
    double avg_flops = (2.0 * N) / avg_time;

    fprintf(file, "\nAverages over %d repetitions:\n", reps);
    fprintf(file, "Time = %.6f sec\n", avg_time);
    fprintf(file, "Bandwidth = %.2f GB/s\n", avg_gb_per_sec);
    fprintf(file, "FLOPS = %.2e\n", avg_flops);

    if (N > 100000000) printf("Computation complete. Results written to %s\n", filename);

    free(pA);
    free(pB);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <vector_size> <repetitions>\n", argv[0]);
        return 1;
    }

    long N = atol(argv[1]);
    int reps = atoi(argv[2]);

    char filename[256];
    snprintf(filename, sizeof(filename), "results_c3_%ld.txt", N);
    
    run_bdp(N, reps, filename);

    return 0;
} 