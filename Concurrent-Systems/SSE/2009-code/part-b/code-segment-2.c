#include <immintrin.h>

/* array "vals" is aligned by 16 bytes */
void code_segment_2(float * vals) {
    for (int i = 0; i < 1027; i++) {
        vals[i] = vals[i] / 3.14159265357;
    }
}

/* array "vals" is aligned by 16 bytes */
void vec_code_segment_2(float * vals) {
    const float pi = 3.14159265357; // for readability purposes
    __m128 vf32_pi = _mm_set1_ps(pi);
    for (int i = 0; i < 1024; i+=4) {
        __m128 vf32_vals = _mm_load_ps(&vals[i]);
        vf32_vals = _mm_div_ps(vf32_vals, vf32_pi);
        _mm_store_ps(&vals[i], vf32_vals);
    }
    // unroll the loop for efficiency since gauranteed 3 values
    vals[1024] = vals[1024] / pi;
    vals[1025] = vals[1025] / pi;
    vals[1026] = vals[1026] / pi;
}
