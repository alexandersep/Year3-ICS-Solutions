#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>


// [   0     ,      0     ,     0      ,     0     ] // ignore last 0
// [pixels[i], pixels[i+1], pixels[i+2], pixels[i+3]] // ignore pixels[i+3]
// ------------------------------------------------------ // add them together vertically
// while loop, do the same against the next pixels and the answer of the add computation.
float rgb_sum_product(float* pixels) {
    __m128 v4sum_pixels = _mm_setzero_ps();
    // array must be divisible by 3, and length 1011, last value of v4sum_pixels is undefined
    for (int i = 0; i < 1011; i+=3) {  
        __m128 v4pixelsi = _mm_loadu_ps(pixels+i); // first three pixels only matter
        v4sum_pixels = _mm_add_ps(v4pixelsi, v4sum_pixels); 
    }
    __m128 v4product_pixels = _mm_mul_ps(v4sum_pixels, 
                                         _mm_shuffle_ps(v4sum_pixels, 
                                                        v4sum_pixels, _MM_SHUFFLE(0, 3, 2, 1)));
    v4product_pixels = _mm_mul_ps(v4product_pixels, 
                                  _mm_shuffle_ps(v4sum_pixels, 
                                                 v4sum_pixels, _MM_SHUFFLE(1, 0, 3, 2)));
    return _mm_cvtss_f32(v4product_pixels); 
}

int main() {
    const int SIZE = 1011;
    float* pixels = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        pixels[i] = i; 
    }
    printf("sum of rgb = %f\n", rgb_sum_product(pixels));
    free(pixels);
    return 0;
}
