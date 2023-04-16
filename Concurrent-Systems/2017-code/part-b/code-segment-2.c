#include <immintrin.h>

float find_max(float * array, int size) {
    float max = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

float vec_find_max(float* array, int size) {
    __m128 vf32_max = _mm_load1_ps(&array[0]);
    const int size_minus_three = size - 3;
    int i;
    for (i = 1; i < size_minus_three; i+=4) {
        __m128 vf32_array = _mm_loadu_ps(&array[i]);
        vf32_max = _mm_max_ps(vf32_array, vf32_max); 
    }
    vf32_max = _mm_max_ps(vf32_max, _mm_shuffle_ps(vf32_max, vf32_max, _MM_SHUFFLE(0,0,3,2)));
    vf32_max = _mm_max_ps(vf32_max, _mm_shuffle_ps(vf32_max, vf32_max, _MM_SHUFFLE(0,0,0,1)));
    float max = _mm_cvtss_f32(vf32_max);
    for (/* i = i */; i < size; i++) {
        if (array[i] > max) { 
            max = array[i]; 
        }
    }
    return max;
}
