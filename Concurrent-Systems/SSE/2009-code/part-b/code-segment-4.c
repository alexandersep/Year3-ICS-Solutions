/* Similar to Concurrent-Systems/2017-code/part-b/code-segment-2.c */
#include <stdio.h>
#include <immintrin.h>

/* array "vals" has unknown alignment */
/* "a" variable is unknown, will assume it's a factor */
void code_segment_4(float* vals, float factor, int size) {
    for (int i = 0; i < size; i++) {
        float v = vals[i];
        vals[i] = 1.0/(v * v) + factor * 2.0;
    }
}

/* array "vals" has unknown alignment */
void vec_code_segment_4(float* vals, float factor, int size) {
    __m128 vf32_one = _mm_set1_ps(1.0);
    __m128 vf32_two = _mm_add_ps(vf32_one, vf32_one); 
    __m128 vf32_factor = _mm_mul_ps(_mm_set1_ps(factor), vf32_two);
    int i;
    int size_minus_three = size - 3; 
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_v = _mm_loadu_ps(&vals[i]); 
        vf32_v = _mm_mul_ps(vf32_v, vf32_v);
        vf32_v = _mm_div_ps(vf32_one, vf32_v);
        vf32_v = _mm_add_ps(vf32_v, vf32_factor);
        _mm_storeu_ps(&vals[i], vf32_v);
    }
    float computed_factor = factor * 2.0;
    for (/* i = i */; i < size; i++) {
        float v = vals[i];
        vals[i] = 1.0/(v * v) + computed_factor; 
    }
}
