#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

// b will be the resulting answer from compute
void compute(float* a, float* b) {
    const float PI = 3.14159;
    const float ONE = 1.0;
    __m128 v4PI = _mm_set1_ps(PI); // set four lanes to PI
    __m128 v4ONE = _mm_set1_ps(ONE); // set four lanes to ONE
    int i;
    for (i = 0; i < 1021; i+=4) { // i < 1024 - 3 = 1021
        __m128 v4a = _mm_loadu_ps(a+i); // load the next four location in a 
        __m128 v4aMulv4a = _mm_mul_ps(v4a, v4a); // multiply v4a with itself
        __m128 v4ONE_div_v4aMulv4a = _mm_div_ps(v4ONE, v4aMulv4a); // v4ONE / (v4a * v4a)
        __m128 v4b = _mm_add_ps(v4ONE_div_v4aMulv4a,v4PI); // divAns + PI
        _mm_storeu_ps(b+i, v4b); 
    }
    // postloop not required as 1024 % 4 == 0 
}

int main() {
    const int SIZE = 1024; 
    const float multiplier = 1.5f;
    float* a = malloc(sizeof(float) * SIZE); 
    float* b = malloc(sizeof(float) * SIZE); 
    // set values for testing
    const float scalar = 10234.343;
    for (int i = 0; i < SIZE; i++) {
        a[i] = i+scalar;
    }
    compute(a, b);
    for (int i = SIZE - 1; i >= 0; i--) {
        printf("b[%d] = %.8f\n", i, b[i]);
    }
    free(a);
    free(b);
    return 0;
}
