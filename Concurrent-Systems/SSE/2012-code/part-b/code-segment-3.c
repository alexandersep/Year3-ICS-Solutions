#include <immintrin.h>

float dot_product(float * a, float * b, int s) {
    float result = 0.0;
    int size = s * 4;
    for (int i = 0; i < size; i++) {
        result = result + a[i] * b[i];
    }
    return result;
}

// Source: https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction
// Fast horiztonal sum, equivalent of 2 hadd's and a lower 32 bit extraction
static inline float hsum_ps_sse3(__m128 v4a) {
    __m128 shuf = _mm_movehdup_ps(v4a);
    __m128 sums = _mm_add_ps(v4a, shuf);
    shuf        = _mm_movehl_ps(shuf, sums);
    sums        = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

float vec_dot_product(float * a, float * b, int s) {
    int i;
    int size = s * 4;
    int size_minus_three = size - 3;
    __m128 vf32_result = _mm_setzero_ps();
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_a = _mm_loadu_ps(&a[i]);
        __m128 vf32_b = _mm_loadu_ps(&b[i]);
        __m128 vf32_mul = _mm_mul_ps(vf32_a, vf32_b);
        vf32_result = _mm_add_ps(vf32_result, vf32_mul);
    }
    float result = hsum_ps_sse3(vf32_result);
    for (/* i = i */; i < size; i++) {
        result = result + a[i] * b[i];
    }
    return result;
}
