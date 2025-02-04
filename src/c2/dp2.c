#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Hint to compiler that arrays don't overlap
float dp_unroll(long N, float * restrict pA, float * restrict pB) {
    float R1 = 0.0f, R2 = 0.0f, R3 = 0.0f, R4 = 0.0f;
    float R5 = 0.0f, R6 = 0.0f, R7 = 0.0f, R8 = 0.0f;
    long j;

    // Main loop with 8-way unrolling
    #pragma GCC ivdep
    for (j = 0; j < N - 7; j += 8) {
        R1 += pA[j] * pB[j];
        R2 += pA[j + 1] * pB[j + 1];
        R3 += pA[j + 2] * pB[j + 2];
        R4 += pA[j + 3] * pB[j + 3];
        R5 += pA[j + 4] * pB[j + 4];
        R6 += pA[j + 5] * pB[j + 5];
        R7 += pA[j + 6] * pB[j + 6];
        R8 += pA[j + 7] * pB[j + 7];
    }

    // Handle remaining elements
    float R = 0.0f;
    for (; j < N; j++) {
        R += pA[j] * pB[j];
    }

    // Combine all partial sums
    return (R1 + R2 + R3 + R4) + (R5 + R6 + R7 + R8) + R;
}

void run_dp_unroll(long N, int reps, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s for writing.\n", filename);
        return;
    }

    // Align memory to cache line boundary for better performance
    float *pA = (float *)aligned_alloc(64, N * sizeof(float));
    float *pB = (float *)aligned_alloc(64, N * sizeof(float));
    if (!pA || !pB) {
        printf("Memory allocation failed\n");
        if (pA) free(pA);
        if (pB) free(pB);
        fclose(file);
        return;
    }

    for (long i = 0; i < N; i++) {
        pA[i] = 1.0f;
        pB[i] = 1.0f;
    }

    struct timespec start, end;
    double total_time = 0.0;

    for (int r = 0; r < reps; r++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        float result = dp_unroll(N, pA, pB);
        clock_gettime(CLOCK_MONOTONIC, &end);

        double elapsed = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1e9;
        total_time += elapsed;

        // Calculate performance metrics
        double gb_per_sec = (2.0 * N * sizeof(float)) / (elapsed * 1e9);  // 2 arrays read
        double flops = (2.0 * N) / elapsed;  // 1 multiply + 1 add per iteration
        
        fprintf(file, "Rep %d: Time = %.6f sec, Result = %.6f, Bandwidth = %.2f GB/s, FLOPS = %.2e\n",
                r + 1, elapsed, result, gb_per_sec, flops);
    }

    double avg_time = total_time / reps;
    double avg_gb_per_sec = (2.0 * N * sizeof(float)) / (avg_time * 1e9);
    double avg_flops = (2.0 * N) / avg_time;

    fprintf(file, "\nAverages over %d repetitions:\n", reps);
    fprintf(file, "Time = %.6f sec\n", avg_time);
    fprintf(file, "Bandwidth = %.2f GB/s\n", avg_gb_per_sec);
    fprintf(file, "FLOPS = %.2e\n", avg_flops);

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
    snprintf(filename, sizeof(filename), "results_c2_%ld.txt", N);
    
    run_dp_unroll(N, reps, filename);

    return 0;
} 