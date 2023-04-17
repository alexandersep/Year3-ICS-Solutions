/* 2010 had no functions, I will make them for testing purposes */
#include <immintrin.h>

/* array "a" is aligned by 16 bytes */
void code_segment_1(float * a) {
    for (int i = 0; i < 1047; i++) {
        a[i] = a[i] + a[i+1];
    }
}

/* array "a" is aligned by 16 bytes */
void vec_code_segment_1(float * a) {
    int i;
    for (i = 0; i < 1044; i+=4) {
        __m128 vf32_a = _mm_load_ps(&a[i]);
        __m128 vf32_a_plus_one = _mm_loadu_ps(&a[i+1]); // i+1 messes up alignment
        vf32_a = _mm_add_ps(vf32_a, vf32_a_plus_one);
        _mm_store_ps(&a[i], vf32_a);
    }
    for (/* i = i */; i < 1047; i++) {
        a[i] = a[i] + a[i+1];
    }
}
