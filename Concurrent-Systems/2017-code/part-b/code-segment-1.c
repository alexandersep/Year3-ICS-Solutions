#include <immintrin.h>

void compute(float * a, float * b) {
    for (int i = 0; i < 1024; i++) {
        b[i] = (1.0/(a[i] * a[i])) + 3.14159;
    }
}

// b will be the resulting answer from compute
void vec_compute(float * a, float * b) {
    const float PI = 3.14159;
    const float ONE = 1.0;
    __m128 vf32_PI = _mm_set1_ps(PI); // set four lanes to PI
    __m128 vf32_ONE = _mm_set1_ps(ONE); // set four lanes to ONE
    for (int i = 0; i < 1024; i+=4) { 
        __m128 vf32_a = _mm_loadu_ps(&a[i]); // load the next four location in a 
        __m128 vf32_mul = _mm_mul_ps(vf32_a, vf32_a); 
        __m128 vf32_div = _mm_div_ps(vf32_ONE, vf32_mul); 
        __m128 vf32_b = _mm_add_ps(vf32_div,vf32_PI); 
        _mm_storeu_ps(&b[i], vf32_b); 
    }
}
