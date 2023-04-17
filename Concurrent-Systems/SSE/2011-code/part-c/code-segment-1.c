/* Similar to Concurrent-Systems/2016-code/part-c/code-segment-1.c */ 
#include <immintrin.h>

// I'm assuming the function should be void since it returns nothing
void do_calc(float * a, float * b, float * c, float factor, int size) {
    for (int i = 0; i < size; i++) {
        a[i] = b[i] + c[i] * factor;
    }
}

// I'm assuming the function should be void since it returns nothing
void vec_do_calc(float * a, float * b, float * c, float factor, int size) {
    __m128 vf32_factor = _mm_set1_ps(factor);
    int i;
    int size_minus_three = size - 3;
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_c = _mm_loadu_ps(&c[i]);
        vf32_c = _mm_mul_ps(vf32_c, vf32_factor);
        __m128 vf32_b = _mm_loadu_ps(&b[i]);
        vf32_c = _mm_add_ps(vf32_c, vf32_b); 
        _mm_storeu_ps(&a[i], vf32_c);
    }
    for (/* i = i */; i < size; i++) {
        a[i] = b[i] + c[i] * factor;
    }
}
