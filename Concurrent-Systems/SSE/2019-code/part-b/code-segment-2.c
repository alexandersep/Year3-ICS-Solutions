/* Similar to Concurrent-Systems/SSE/2016-code/part-c/code-segment-2.c */
#include <immintrin.h>

float sums(float* a, int size) {
    float sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum = sum + a[i];
    }
    return sum;
}

// Source: https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction
// Equivalent to 2 hadd's with a 32 bit lower extraction
static inline float hsum_ps_sse3(__m128 v4a) {
    __m128 shuf = _mm_movehdup_ps(v4a);
    __m128 sums = _mm_add_ps(v4a, shuf);
    shuf        = _mm_movehl_ps(shuf, sums);
    sums        = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

float vec_sums(float* a, int size) {
    int i;
    __m128 vf32_sum = _mm_setzero_ps();
    int size_minus_three = size - 3;
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_a = _mm_loadu_ps(&a[i]);
        vf32_sum = _mm_add_ps(vf32_a, vf32_sum);
    }
    float sum = hsum_ps_sse3(vf32_sum);
    for (/* i = i */; i < size; i++) {
        sum = sum + a[i];
    }
    return sum;
}
