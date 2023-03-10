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

float dot_product(float* restrict a, float* restrict b, int size) {
    const int size_minus_three = size - 3;
    __m128 v4sum = _mm_setzero_ps();
    int i;
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 v4a = _mm_loadu_ps(a+i);
        __m128 v4b = _mm_loadu_ps(b+i);
        __m128 v4a_mul_v4b = _mm_mul_ps(v4a, v4b);
        v4sum = _mm_add_ps(v4sum, v4a_mul_v4b);
    }
    float sum = hsum(v4sum); 
    // postloop
    // i = i;
    for (; i < size; i++) {
        sum = sum + a[i] * b[i];
    }
    return sum;
}

int main() {
    const int SIZE = 1025; 
    float* a = malloc(sizeof(float) * SIZE); 
    float* b = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
        b[i] = i; 
    }
    printf("sum = %.2f\n", dot_product(a,b,SIZE));
    free(a);
    free(b);
    return 0;
}
