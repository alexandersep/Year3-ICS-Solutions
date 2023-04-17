#include <immintrin.h>

int max_index(float* array) {
    float max = array[0];
    int max_idx = 0;
    for (int i = 1; i < 4096; i++) {
        if (array[i] > max) {
            max_idx = i;
        }
    }
    //printf("max_idx = %d\n\n", max_idx);
    return max_idx;
}

// optimised vectorised approach, starts at end of array instead 
// as max (vf32_max vector version) never gets updated, and hence
// the highest array[i] value higher than array[0] is the answer
int vec_max_index(float* array) {
    __m128 vf32_max = _mm_set1_ps(array[0]);
    int max_idx = 0;
    for (int i = 4095; i >= 4; i-=4) {
        __m128 vf32_array_i = _mm_loadu_ps(&array[i-3]);
        __m128 vf32_cmpgt_max_array = _mm_cmpgt_ps(vf32_array_i, vf32_max);
        int movemask = _mm_movemask_ps(vf32_cmpgt_max_array);
        if (movemask) {
            if (movemask == 1) {
                max_idx = i - 3;
            }
            else if (movemask <= 3) {
                max_idx = i - 2;
            }
            else if (movemask <= 7) {
                max_idx = i - 1;
            }
            else if (movemask <= 15) {
                max_idx = i; 
            }
            break;
        }
    }
    return max_idx;
}
