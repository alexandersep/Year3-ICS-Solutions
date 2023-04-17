#include <immintrin.h>

/* array "vals" is 16-byte aligned */
void code_segment_3(float * vals) {
    for (int i = 1; i < 1024; i++) {
        vals[i] = vals[i-1];
    }
}

/* array "vals" is 16-byte aligned */
void vec_code_segment_3(float * vals) {
    __m128 vf32_vals = _mm_set1_ps(vals[0]);
    for (int i = 0; i < 1024; i+=4) {
        _mm_store_ps(&vals[i], vf32_vals);
    }
}
