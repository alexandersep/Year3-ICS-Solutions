/* Similar to Concurrent-Systems/2011-code/part-b/code-segment-2.c */
#include <immintrin.h>

/* array "pow" is 16-byte aligned */
/* number contains uninitialized value, hence I will assume it passed as a parameter */
void code_segment_3(float number, float * pow) {
    float power = 1.0;
    for (int i = 0; i < 256; i++) {
        pow[i] = power;
        power = power * number;
    }
}

/* array "pow" is 16-byte aligned */
/* number contains uninitialized value, hence I will assume it passed as a parameter */
void vec_code_segment_3(float number, float * pow) {
    float number_2 = number * number;
    float number_3 = number_2 * number;
    float number_4 = number_3 * number;
    __m128 vf32_power = _mm_setr_ps(1.0, number, number_2, number_3);
    __m128 vf32_num = _mm_set1_ps(number_4);
    for (int i = 0; i < 256; i+=4) {
        _mm_store_ps(&pow[i], vf32_power);
        vf32_power = _mm_mul_ps(vf32_power, vf32_num);
    }
}
