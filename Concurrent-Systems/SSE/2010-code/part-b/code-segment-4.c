/* Similar to Concurrent-Systems/2017-code/part-b/code-segment-2.c */
#include <immintrin.h>

/* array "numbers" is 16-byte aligned */
/* assuming a[i] is meant to be numbers[i] */
float code_segment_4(float* numbers) {
    float min = numbers[0];
    for (int i = 1; i < 1000; i++) {
        if (numbers[i] < min) {
            min = numbers[i];
        }
    }
    return min;
}

/* array "numbers" is 16-byte aligned */
/* assuming a[i] is meant to be numbers[i] */
float vec_code_segment_4(float* numbers) {
    __m128 vf32_min = _mm_set1_ps(numbers[0]);
    int i;
    for (i = 1; i < 1000; i+=4) { /* 1000 - 3 */ 
        __m128 vf32_numbers = -_mm_loadu_ps(&numbers[i]);
        vf32_min = _mm_max_ps(vf32_numbers, vf32_min);
    }
    vf32_min = _mm_max_ps(vf32_min, _mm_shuffle_ps(vf32_min, vf32_min, _MM_SHUFFLE(0,0,0,2)));
    vf32_min = _mm_max_ps(vf32_min, _mm_shuffle_ps(vf32_min, vf32_min, _MM_SHUFFLE(0,0,0,1)));
    float min = _mm_cvtss_f32(vf32_min);
    /* unrolled for loop */
    if (numbers[997] < min) {
        min = numbers[997];
    }
    if (numbers[998] < min) {
        min = numbers[998];
    }
    if (numbers[999] < min) {
        min = numbers[999];
    }
    return min;
}
