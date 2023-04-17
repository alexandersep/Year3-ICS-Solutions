#include <immintrin.h>

void add_scaled(float* a, float* b, float* c, float factor) {
    for (int i = 0; i < 1024; i++) {
        a[i] = b[i] + c[i] * factor;
    }
}

void vec_add_scaled(float* a, float* b, float* c, float factor) {
    __m128 vf32_factor = _mm_set1_ps(factor);
    for (int i = 0; i < 1024; i+=4) { 
        __m128 vf32_b = _mm_loadu_ps(&b[i]);
        __m128 vf32_c = _mm_loadu_ps(&c[i]);
        __m128 vf32_mul = _mm_mul_ps(vf32_c, vf32_factor);
        __m128 vf32_result = _mm_add_ps(vf32_b, vf32_mul);
        _mm_storeu_ps(&a[i], vf32_result);
    }
}
