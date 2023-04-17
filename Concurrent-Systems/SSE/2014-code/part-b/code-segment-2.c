#include <immintrin.h>

// b length must be 4100 length for safety
void compute(float *restrict a, float *restrict b) {
    for (int i = 0; i < 4096; i++) {
        b[i] = a[i] * 3.0 + b[i+4];
    }
}

// b length must be 4100 length for safety
void vec_compute(float *restrict a, float *restrict b) {
    __m128 vf32_three = _mm_set1_ps(3.0);
    for (int i = 0; i < 4096; i++) {
        __m128 vf32_a = _mm_loadu_ps(&a[i]);
        __m128 vf32_b_i_plus_four = _mm_loadu_ps(&b[i+4]);
        __m128 vf32_mul = _mm_mul_ps(vf32_a, vf32_three);
        __m128 vf32_add = _mm_add_ps(vf32_mul, vf32_b_i_plus_four);
        _mm_storeu_ps(&b[i], vf32_add);
    }
}
