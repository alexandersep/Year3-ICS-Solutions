#include <immintrin.h>

void compute_1(int* array, int SIZE) {
    // array is 16-byte unaligned address
    int i = 1;
    while (i < SIZE) {
        array[i] = array[i] + array[i-1];
        i = i + 1;
    }
}

void vec_compute_1(int* array, int SIZE) {
    int i;
    int size_minus_three = SIZE - 3;
    for (i = 0; i < size_minus_three; i+=4) {
        __m128i vf32_array = _mm_loadu_si128((__m128i *)&array[i]);
        __m128i vf32_array_plus_one = _mm_loadu_si128((__m128i *)&array[i+1]);
        _mm_storeu_si128((__m128i *)&array[i], _mm_add_epi32(vf32_array, vf32_array_plus_one));
    }
    for (/* i = i */; i < SIZE; i++) {
        array[i] = array[i] + array[i-1];
    }
}
