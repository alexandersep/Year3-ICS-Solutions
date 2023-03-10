#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

// efficient approach for calculating horizontal sum of vector in SSE3
static inline float hsum(__m128 v4) {
    __m128 shuf = _mm_movehdup_ps(v4);
    __m128 sums = _mm_add_ps(v4, shuf);
    shuf = _mm_movehl_ps(shuf,sums);
    sums = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

/*
// naive approach, but simpler and shorter code
static inline float hsum(__m128 v4) {
    v4 = _mm_hadd_ps(v4, v4);    
    v4 = _mm_hadd_ps(v4, v4);    
    return _mm_cvtss_f32(v4);
}
*/

float dot_product(float* restrict a, float* restrict b) {
    __m128 v4sum = _mm_setzero_ps();
    int i;
    for (i = 0; i < 4096; i+=4) { // 4096 % 4, no need for postloop
        __m128 v4a = _mm_loadu_ps(a+i);
        __m128 v4b = _mm_loadu_ps(b+i);
        __m128 v4a_mul_v4b = _mm_mul_ps(v4a, v4b);
        v4sum = _mm_add_ps(v4sum, v4a_mul_v4b);
    }
    return hsum(v4sum);
}

int main() {
    const int SIZE = 4096; 
    float* a = malloc(sizeof(float) * SIZE); 
    float* b = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
        b[i] = i; 
    }
    printf("sum = %.2f\n", dot_product(a,b));
    free(a);
    free(b);
    return 0;
}
