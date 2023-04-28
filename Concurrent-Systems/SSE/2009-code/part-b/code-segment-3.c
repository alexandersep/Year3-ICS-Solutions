#include <immintrin.h>

/* array "vals" is 16-byte aligned */
void code_segment_3(float * vals) {
    for (int i = 1; i < 1024; i++) {
        vals[i] = vals[i-1];
    }
}

/* array "vals" is 16-byte aligned */
void vec_code_segment_3(float * vals) {
    __m128 vf32_vals = _mm_load_ss(&vals[0]); // [1, 2, 3, 4]
    vf32_vals = _mm_shuffle_ps(vf32_vals, vf32_vals, _MM_SHUFFLE(0,0,0,0)); // optimise out set_ps
    for (int i = 0; i < 1024; i+=4) {
        _mm_store_ps(&vals[i], vf32_vals);
    }
}
