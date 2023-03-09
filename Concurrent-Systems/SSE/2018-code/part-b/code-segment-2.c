#include <stdio.h>
#include <immintrin.h>

void compute(float* array, int SIZE, float multiplier) {
    int i;
    __m128 v4multipler = _mm_set1_ps(multiplier);
    __m128 v4SIZE = _mm_set1_ps(SIZE);
    for (i = SIZE - 4; i >= 0; i-=4) {
        __m128 v4array = _mm_load_ps(&(array[i]));
        __m128 v4array_mul_v4multiplier = _mm_mul_ps(v4array, v4multipler);
        __m128 v4result = _mm_div_ps(v4array_mul_v4multiplier,v4SIZE);
        _mm_storeu_ps(&(array[i]), v4result);
    }
     for (; i >= 0; i--) {
        array[i] = (array[i] * multiplier) / SIZE;
    }
}

int main() {
    const int SIZE = 4; 
    const float multiplier = 1.5f;
    float*  array = _mm_malloc(sizeof(float) * SIZE, 16);
    array[0] = 100.5;
    array[1] = 200.666;
    array[2] = 300.125;
    array[3] = 400.3333;
    for (int i = SIZE - 1; i >= 0; i--) {
        printf("array[%d] = %.2f\n", i, array[i]);
    }
    printf("\n");
    compute(array, SIZE, multiplier);
    for (int i = SIZE - 1; i >= 0; i--) {
        printf("array[%d] = %.2f\n", i, array[i]);
    }
    _mm_free(array);
    return 0;
}
