#include <immintrin.h>

// array is aligned
int compute_3(int* array, int SIZE) {
    int max = 0;
    for (int i = 0; i < SIZE; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

// array is aligned
int vec_compute_3(int* array, int SIZE) {
    // array is 16-byte aligned address
    __m128i vf32_max = _mm_setzero_si128();
    int i;
    int size_minus_three = SIZE - 3; // optimise for "for loop"
    for(i = 0; i < size_minus_three; i+=4) {
        __m128i vf32_array = _mm_load_si128((__m128i*)&(array[i]));
        vf32_max = _mm_max_epi32(vf32_array, vf32_max);
    }
    // select the maximum value which is in one of the four lanes in v4max
    vf32_max = _mm_max_epi32(vf32_max, _mm_shuffle_epi32(vf32_max, _MM_SHUFFLE(0, 0, 3, 2)));
    vf32_max = _mm_max_epi32(vf32_max, _mm_shuffle_epi32(vf32_max, _MM_SHUFFLE(0, 0, 0, 1)));
    int max = _mm_cvtsi128_si32(vf32_max); 
    for(/* i = i */; i < SIZE; i++) {
        if (array[i] > max) { max = array[i]; }
    }
    return max;
}
