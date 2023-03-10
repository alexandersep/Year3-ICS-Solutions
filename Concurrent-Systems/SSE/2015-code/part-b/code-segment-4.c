#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

// TODO: This is not the correct function

// It is the skieloton that I have yet to add
// find maximum of array
float find_max(float* array, int size) {
    __m128 v4max = _mm_load1_ps(array); // load first four values
    const int size_minus_three = size - 3;
    int i;
    for (i = 1; i < size_minus_three; i+=4) {
        __m128 v4array = _mm_loadu_ps(array+i);
        v4max = _mm_max_ps(v4array, v4max); 
    }
    v4max = _mm_max_ps(v4max, _mm_shuffle_ps(v4max, v4max, _MM_SHUFFLE(0,0,3,2)));
    v4max = _mm_max_ps(v4max, _mm_shuffle_ps(v4max, v4max, _MM_SHUFFLE(0,0,0,1)));
    float max = _mm_cvtss_f32(v4max);
    // i = i;
    for (; i < size; i++) {
        if (array[i] > max) { 
            max = array[i]; 
        }
    }
    return max;
}

int main() {
    const int SIZE = 1025; 
    const float multiplier = 1.5f;
    float* array = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        array[i] = i;
    }
    printf("max = %f\n", find_max(array,SIZE));
    free(array);
    return 0;
}
