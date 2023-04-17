#include <immintrin.h>

void compute_2(float* array, int SIZE, float multiplier) {
    for (int i = SIZE - 1; i >= 0; i--) {
        array[i] = (array[i] * multiplier) / SIZE;
    }
}

void vec_compute_2(float* array, int SIZE, float multiplier) {
    int i;
    __m128 vf32_multipler = _mm_set1_ps(multiplier);
    __m128 vf32_SIZE = _mm_set1_ps(SIZE);
    int size_minus_three = SIZE - 3;
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_array = _mm_loadu_ps(&(array[i]));
        __m128 vf32_mul = _mm_mul_ps(vf32_array, vf32_multipler);
        __m128 vf32_result = _mm_div_ps(vf32_mul,vf32_SIZE);
        _mm_storeu_ps(&(array[i]), vf32_result);
    }
     for (/* i = i */; i < SIZE; i++) {
        array[i] = (array[i] * multiplier) / SIZE;
    }
}
