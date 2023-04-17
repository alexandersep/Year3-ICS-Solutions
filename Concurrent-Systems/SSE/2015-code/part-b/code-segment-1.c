#include <immintrin.h>

/* code segment 1 */
void mul_neighbour(float * array, int size) {
    for (int i = 0; i < size-1; i++) {
        array[i] = array[i] * array[i+1];
    }
}

/* vectorised code segment 1 */
void vec_mul_neighbour(float * array, int size) {
    int i;
    int size_minus_four = size-4; // optimised var loop
    for (i = 0; i < size_minus_four; i+=4) {
        __m128 vf32_array_i = _mm_loadu_ps(&array[i]);
        __m128 vf32_array_i_plus_one = _mm_loadu_ps(&array[i+1]);
        __m128 vf32_array = _mm_mul_ps(vf32_array_i, vf32_array_i_plus_one);
        _mm_storeu_ps(&array[i], vf32_array);
    }
    for (/* i = i */; i < size-1; i++) {
        array[i] = array[i] * array[i+1];
    }
}
