#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float dp(long N, float *pA, float *pB) {
    float R = 0.0;
    int j;
    for (j=0; j<N; j++)
        R += pA[j]*pB[j];
    return R;
}

void run_dp(long N, int reps, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s for writing.\n", filename);
        return;
    }

    float *pA = (float *)malloc(N * sizeof(float));
    float *pB = (float *)malloc(N * sizeof(float));
    if (!pA || !pB) {
        printf("Memory allocation failed\n");
        fclose(file);
        return;
    }

    for (long i = 0; i < N; i++) {
        pA[i] = 1.0;
        pB[i] = 1.0;
    }

    struct timespec start, end;
    double total_time = 0.0;

    for (int r = 0; r < reps; r++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        float result = dp(N, pA, pB);
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
    snprintf(filename, sizeof(filename), "results_c1_%ld.txt", N);
    
    run_dp(N, reps, filename);

    return 0;
} 