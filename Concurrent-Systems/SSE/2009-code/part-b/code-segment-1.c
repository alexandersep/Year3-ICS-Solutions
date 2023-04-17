/* 2009 had no functions, I will make them for testing purposes */
#include <xmmintrin.h>

/* Fibonnacci is inherently a data dependent algorithm requiring the previous two results in the next result
 * You cannot use SSE effectively with this algorithm as SSE does 4 calculations at a time, and would work
 * best doing things 1 at a time with 2 previous values. The original code implementation is inferior as it
 * uses to memory accesses to get previous 2 results when it would be better to use 2 variables to avoid any
 * memory calls. The below vectorised code using intel intrinsics for calculating things mainly one at a time
 * in a four lane float vector. It uses _mm_load_ps by calculating things four at a time ensuring 16-byte alignment
 * throughout the programme and it does not need a postloop because 1000 % 4 == 0 */

/* array "fibs" is aligned by 16 bytes */
void code_segment_1(float * fibs) {
    fibs[0] = 0;
    fibs[1] = 1;
    for (int i = 2; i < 1000; i++) {
        fibs[i] = fibs[i-1] + fibs[i-2];
    }
}

/* array "fibs" is aligned by 16 bytes */
void vec_code_segment_1(float * fibs) {
    fibs[0] = 0;
    fibs[1] = 1;
    /* note fibs[2], fibs[3] and fibs[4] is undefined */
    int i;
    for (i = 2; i < 1000; i+=4) { // 1000 - 3 (load 4 times, but only use 2 at a time, so 1000 - 3 - 1 = 996
        __m128 vf32_fibs_i_minus_one = _mm_load_ps(&fibs[i-2]);
        __m128 vf32_shifted_left = _mm_shuffle_ps(vf32_fibs_i_minus_one, vf32_fibs_i_minus_one, _MM_SHUFFLE(3, 2, 0, 1));

        __m128 vf32_prev = _mm_add_ss(vf32_shifted_left, vf32_fibs_i_minus_one);
        _mm_store_ss(&fibs[i], vf32_prev); 

        __m128 vf32_next = _mm_add_ss(vf32_shifted_left, vf32_prev); 
        _mm_store_ss(&fibs[i+1], vf32_next); 

        vf32_prev = _mm_add_ss(vf32_prev, vf32_next);
        _mm_store_ss(&fibs[i+2], vf32_prev); 

        vf32_next = _mm_add_ss(vf32_prev, vf32_next);
        _mm_store_ss(&fibs[i+3], vf32_next); 
    }
}
