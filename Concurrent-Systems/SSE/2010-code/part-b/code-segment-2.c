#include <immintrin.h>

/* array "vals" is aligned by 16 bytes */
void code_segment_2(float * vals) {
    const float pi = 3.14159265357;
    const int degrees = 180;
    for (int i = 0; i < 1027; i++) {
        vals[i] = (degrees * vals[i]) / pi;
    }
}

/* array "vals" is aligned by 16 bytes */
void vec_code_segment_2(float * vals) {
    const float pi = 3.14159265357;
    const int degrees = 180;
    __m128 vf32_pi = _mm_set1_ps(pi);
    __m128 vf32_degrees = _mm_set1_ps(degrees);
    for (int i = 0; i < 1024; i+=4) { // 1027 - 3 = 1024
        __m128 vf32_vals = _mm_load_ps(&vals[i]);
        vf32_vals = _mm_mul_ps(vf32_degrees, vf32_vals);
        vf32_vals = _mm_div_ps(vf32_vals, vf32_pi);
        _mm_storeu_ps(&vals[i], vf32_vals);
    }
    // unroll the loop for efficiency since gauranteed 3 values
    vals[1024] = (degrees * vals[1024]) / pi;
    vals[1025] = (degrees * vals[1025]) / pi;
    vals[1026] = (degrees * vals[1026]) / pi;
}
