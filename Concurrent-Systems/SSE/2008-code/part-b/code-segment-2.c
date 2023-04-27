#include <immintrin.h>
#include <stdio.h>
#include <math.h>

/* array "vals" is aligned by 16 bytes */
void code_segment_2(float * vals) {
    float a = 5;
    float b = 3;
    for (int i = 0; i < 4096; i++) {
        vals[i] = 1/b * a/b + 1/(sqrtf(vals[i]));
    }
}

// This is theoretically good, but floats are inconsistent 
// and will not result in 20 bits of preceision and hence 
// the data is unusable and Segementation faults occur 
/* array "vals" is aligned by 16 bytes */
void vec_code_segment_2(float * vals) {
    // constant vector values
    __m128 vf32_one = _mm_set1_ps(1);
    __m128 vf32_a = _mm_set1_ps(5);
    __m128 vf32_b = _mm_set1_ps(3);

    // vector 1 / b 
    __m128 vf32_r1b = _mm_div_ps(vf32_one, vf32_b);
    // vector a / b 
    __m128 vf32_rab = _mm_div_ps(vf32_a, vf32_b); 

    // vector 1 / b * a / b
    __m128 vf32_calc = _mm_mul_ps(vf32_r1b, vf32_rab);
    for (int i = 0; i < 4096; i++) {
        __m128 vf32_rvals = _mm_load_ps(&vals[i]);
        vf32_rvals = _mm_rsqrt_ps(vf32_rvals);
        vf32_calc = _mm_add_ps(vf32_calc, vf32_rvals);
        _mm_store_ps(&vals[i], vf32_calc);
    }
}
