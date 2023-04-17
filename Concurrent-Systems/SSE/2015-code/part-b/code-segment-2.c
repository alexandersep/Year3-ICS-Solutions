#include <immintrin.h>

float dot_product(float *restrict a, float *restrict b) {
    float sum = 0.0;
    for (int i = 0; i < 4096; i++) {
        sum = sum + a[i] * b[i];
    }
    return sum;
}

// efficient approach for calculating horizontal sum of vector in SSE3
static inline float hsum(__m128 v4) {
    __m128 shuf = _mm_movehdup_ps(v4);
    __m128 sums = _mm_add_ps(v4, shuf);
    shuf = _mm_movehl_ps(shuf,sums);
    sums = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

float vec_dot_product(float *restrict a, float *restrict b) {
    __m128 vf32_sum = _mm_setzero_ps();
    int i;
    for (i = 0; i < 4096; i+=4) { // 4096 % 4, no need for postloop
        __m128 vf32_a = _mm_loadu_ps(&a[i]);
        __m128 vf32_b = _mm_loadu_ps(&b[i]);
        __m128 vf32_mul = _mm_mul_ps(vf32_a, vf32_b);
        vf32_sum = _mm_add_ps(vf32_sum, vf32_mul);
    }
    return hsum(vf32_sum);
}
