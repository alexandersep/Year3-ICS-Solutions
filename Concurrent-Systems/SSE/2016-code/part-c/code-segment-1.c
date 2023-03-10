#include <stdio.h>
#include <immintrin.h>

// result will be in a
void add_scaled(float* a, float* b, float* c, float factor) {
    __m128 v4factor = _mm_set1_ps(factor);
    for (int i = 0; i < 1024; i+=4) { // postloop not needed since arrays % 4 
        __m128 v4b = _mm_loadu_ps(b+i);
        __m128 v4c = _mm_loadu_ps(c+i);
        __m128 v4c_mul_v4factor = _mm_mul_ps(v4c, v4factor);
        __m128 result = _mm_add_ps(v4b, v4c_mul_v4factor);
        _mm_storeu_ps(a+i, result);
    }
}

int main() {
    const int SIZE = 1024; 
    const float factor = 1.5f;
    float* a = malloc(sizeof(float) * SIZE); 
    float* b = malloc(sizeof(float) * SIZE); 
    float* c = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        b[i] = i;
        c[i] = i;
    }
    add_scaled(a, b, c, factor);
    for (int i = 0; i < SIZE; i++) {
        printf("a[%d] = %.2f\n", i, a[i]);
    }
    free(a);
    free(b);
    free(c);
    return 0;
}
