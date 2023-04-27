#include <immintrin.h>

float dot_product(float *restrict a, float *restrict b, int size) {
    float sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum = sum + a[i] * b[i];
    }
    return sum;
}

// Source: https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction
// efficient approach for calculating horizontal sum of vector in SSE3
static inline float hsum(__m128 v4) {
    __m128 shuf = _mm_movehdup_ps(v4);
    __m128 sums = _mm_add_ps(v4, shuf);
    shuf = _mm_movehl_ps(shuf,sums);
    sums = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

float vec_dot_product(float* restrict a, float* restrict b, int size) {
    const int size_minus_three = size - 3;
    __m128 vf32_sum = _mm_setzero_ps();
    int i;
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_a = _mm_loadu_ps(a+i);
        __m128 vf32_b = _mm_loadu_ps(b+i);
        __m128 vf32_mul = _mm_mul_ps(vf32_a, vf32_b);
        vf32_sum = _mm_add_ps(vf32_sum, vf32_mul);
    }
    float sum = hsum(vf32_sum); 
    for (/* i = i */; i < size; i++) {
        sum = sum + a[i] * b[i];
    }
    return sum;
}
