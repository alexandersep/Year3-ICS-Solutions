#include <immintrin.h>

void first_1024_powers(float num, float * result) {
    float power = num;
    for (int i = 0; i < 1024; i++) {
        result[i] = power;
        power = power * num;
    }
}

void vec_first_1024_powers(float num, float * result) {
    float num_2 = num * num;
    float num_3 = num_2 * num;
    float num_4 = num_3 * num;
    __m128 vf32_power = _mm_setr_ps(num, num_2, num_3, num_4);
    __m128 vf32_num = _mm_set1_ps(num_4);
    for (int i = 0; i < 1024; i+=4) {
        _mm_storeu_ps(&result[i], vf32_power);
        vf32_power = _mm_mul_ps(vf32_power, vf32_num);
    }
}
