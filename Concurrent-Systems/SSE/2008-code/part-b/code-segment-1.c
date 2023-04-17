/* 2008 had no functions, I will make them for testing purposes */
#include <immintrin.h>
#include <math.h>

/* This code is not vectorisable efficiently. A data dependency exists
 * where the previous value of the array will be used in the current
 * value of the array, vectorising this would require working with
 * one values at a time and would not work well with SSE instructions */

/* array "vals" is 16-byte aligned */
void code_segment_1(float * vals) {
    for (int i = 1; i < 4096; i++) {
        vals[i] = 1/(sqrtf(vals[i-1]));
    }
}
