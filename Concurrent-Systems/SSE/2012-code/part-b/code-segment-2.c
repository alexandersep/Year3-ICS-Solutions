#include <immintrin.h>

/* src and dest memory regions do not overlap (hence restrict */
void memcpy_2(char * restrict dest, char * restrict src, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

void vec_memcpy_2(char * restrict dest, char * restrict src, int size) {
    int i;
    int size_minus_fifteen = size - 15;
    for (i = 0; i < size_minus_fifteen; i+=16) { // 16 characters, 128 bits
        __m128 vf32_src = _mm_loadu_ps((float *)&src[i]);
        _mm_storeu_ps((float *)&dest[i], vf32_src);
    }
    for (/* i = i */; i < size; i++) {
        dest[i] = src[i];
    }
}
