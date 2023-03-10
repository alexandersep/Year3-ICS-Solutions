#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

// An efficient way of summing up a vector
// More efficient than doing hadd twice and getting the
// first element
// equivalent to 
/*
v4a = _mm_hadd_ps(v4a);
v4a = _mm_hadd_ps(v4a);
return _mm_cvtss_f32(v4a);
*/
static inline float hsum_ps_sse3(__m128 v4a) {
    __m128 shuf = _mm_movehdup_ps(v4a);
    __m128 sums = _mm_add_ps(v4a, shuf);
    shuf        = _mm_movehl_ps(shuf, sums);
    sums        = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

void multiply(float ** restrict matrix, float * restrict vec, float * restrict result) {
    for (int i = 0; i < 4096; i++) {
        __m128 v4sum = _mm_setzero_ps();  
        for (int j = 0; j < 4093; j+=4) { // i < 4096 - 3 since j+=4
            __m128 v4vecj = _mm_loadu_ps(&(vec[j]));
            __m128 v4matrixij = _mm_loadu_ps(&(matrix[i][j]));
            __m128 v4vecj_mul_v4matrixij = _mm_mul_ps(v4vecj, v4matrixij);
            v4sum = _mm_add_ps(v4sum, v4vecj_mul_v4matrixij);
        }
        result[i] = hsum_ps_sse3(v4sum); // efficient horiztonal sum vector
    }
}

int main() {
    const int SIZE = 4096;
    float** restrict matrix = malloc(sizeof(float*) * SIZE);
    float* restrict vec = malloc(sizeof(float) * SIZE);
    float* restrict result = malloc(sizeof(float) * SIZE);
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        matrix[i] = malloc(sizeof(float) * SIZE);
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = 5; 
        }
        vec[i] = 6;
    }
    multiply(matrix, vec, result);
    for (int i = 0; i < SIZE; i++) {
        printf("result[%d] = %.2f\n", i, result[i]); 
        free(matrix[i]); // free float* to array before freeing float ** to prevent memory leaks
    }
    free(matrix);
    free(vec);
    free(result);
    return 0;
}
