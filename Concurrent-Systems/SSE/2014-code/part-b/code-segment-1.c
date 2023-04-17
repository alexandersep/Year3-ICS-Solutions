#include <immintrin.h>

/* code segment 1 */
void shift(float * array, int size) {
    for (int i = 1; i < size; i++) {
        array[i] = array[i-1];
    }
}

/* vectorised code segment 1 */
void vec_shift(float * array, int size) {
    int i;
    int size_minus_three = size - 3;
    __m128 vf32_values = _mm_set1_ps(array[0]);
    for (i = 1; i < size_minus_three; i+=4) {
        _mm_storeu_ps(&array[i], vf32_values);
    }
    for (/* i = i */; i < size; i++) {
        array[i] = array[i-1];
    }
}
