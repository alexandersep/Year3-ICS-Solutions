#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

int compute(int* array, int SIZE) {
    // array is 16-byte aligned address
    __m128i v4max = _mm_setzero_si128();
    int i;
    for(i = 0; i < SIZE - 3; i+=4) {
        __m128i v4array = _mm_loadu_si128((__m128i*)&(array[i]));
        v4max = _mm_max_epi32(v4array, v4max);
    }
    // select the maximum value which is in one of the four lanes in v4max
    v4max = _mm_max_epi32(v4max, _mm_shuffle_epi32(v4max, _MM_SHUFFLE(0, 0, 3, 2)));
    v4max = _mm_max_epi32(v4max, _mm_shuffle_epi32(v4max, _MM_SHUFFLE(0, 0, 0, 1)));
    int max = _mm_cvtsi128_si32(v4max); 
    // postloop for when the array is not divsible by 4
    for(/* i = i */; i < SIZE; i++) {
        if (array[i] > max) { max = array[i]; }
    }
    return max;
}

int main() {
    const int SIZE = 4; 
    int*  array = malloc(sizeof(int) * SIZE);
    array[0] = 0;
    array[1] = 2;
    array[2] = 7;
    array[3] = 3;
    printf("max is %d\n", compute(array,SIZE));
    free(array);
    return 0;
}
