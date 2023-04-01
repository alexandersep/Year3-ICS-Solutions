#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

int max_index(float* array) {
    __m128 v4max = _mm_load1_ps(array);
    __m128 v4max_idx = _mm_setzero_ps();
    int max_idx = 0;
    for (int i = 0; i < 4096; i+=4) { // start from 0, to avoid potential undefined lane
        __m128 v4a = _mm_loadu_ps(array+i);
        __m128 v4a_cmpgt_v4max = _mm_cmpgt_ps(v4a, v4max);
        __m128 v4i = _mm_set_ps(i+3, i+2, i+1, i);
        v4max_idx = _mm_max_ps(v4max_idx, v4a);
        v4max_idx = _mm_blend_ps(v4max_idx, v4a_idx, v4a_cmpgt_v4max);
    }
    int result = 0;
    float max = _mm_cvtss_f32(max_val);  // extract the maximum value from max_val
    float* max_idx_ptr = (float*)&v4max_idx;  // cast max_idx to an int pointer
    for (int i = 0; i < 4; i++) {
        if (array[max_idx_ptr[i]] == max) {  // check which of the four indices in max_idx_ptr corresponds to the maximum value
            result = (int) max_idx_ptr[i];
            break;
        }
    }
    return result;
}

/*
int max_index(float* array) {
    float max = array[0];
    int max_idx = 0;
    for (int i = 1; i < 4096; i++) {
        if (array[i] > max) {
            max_idx = i;
        }
    }
    return max_idx;
}
*/

int main() {
    const int SIZE = 4096; 
    float* array = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        array[i] = i;
    }
    printf("max = %f\n", max_index(array));
    free(array);
    return 0;
}
