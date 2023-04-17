#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <time.h>
#include "code-segments.h"

// compute square of distance between two arrays
double diff_square(float * a, float * b, int size) {
    double sum = 0.0;
    for ( int i = 0; i < size; i++ ) {
        double diff = a[i] - b[i];
        //fprintf(stderr, "a[%d]: %f, b[%d]: %f\n", i, a[i], i, b[i]);
        sum = sum + diff * diff;
    }
    return sum;
}

// create a copy of an existing array
float * clone_float_array(float * src, int size) {
    float * dest = malloc(sizeof(float)*size);
    for ( int i = 0; i < size; i++ ) {
        dest[i] = src[i];
    }
    return dest;
}

void test_code_segment_1() {
    const int SIZE = 4096; 
    float* a = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
    }
    float* b = clone_float_array(a, SIZE);

    mul_neighbour(a, SIZE);
    vec_mul_neighbour(b,SIZE);

    double diff = diff_square(a, b, SIZE);
    fprintf(stderr, "mul_neighbour diff: %lf\n", diff);

    free(a);
    free(b);
}

void test_code_segment_2() {
    const int SIZE = 4096; 
    float* a = malloc(sizeof(float) * SIZE); 
    float* b = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
        b[i] = i; 
    }
    float dp = dot_product(a, b);
    float vdp = vec_dot_product(a, b);

    double diff = dp - vdp; 
    fprintf(stderr, "dot_product diff: %lf\n", diff);

    free(a);
    free(b);
}

void test_code_segment_3() {
    const int SIZE = 1011; 
    float* pixels = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        pixels[i] = i*100;
    }
    float* clone_pixels = clone_float_array(pixels, SIZE);

    float rgbp = rgb_sum_product(pixels);
    float vrgbp = vec_rgb_sum_product(clone_pixels);

    double diff = rgbp - vrgbp;
    fprintf(stderr, "rgb_sum_product diff: %lf\n", diff);

    free(pixels);
    free(clone_pixels);
}

void test_code_segment_4() {
    const int SIZE = 4096; 
    float* array = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        array[i] = (float)rand();
    }
    float* clone_array = clone_float_array(array, SIZE);

    int midx = max_index(array);
    int vmidx = vec_max_index(clone_array);

    double diff = midx - vmidx; 
    fprintf(stderr, "max_idx  diff: %lf\n", diff);

    free(array);
    free(clone_array);
}

int main() {
    srand(time(NULL));
    printf("Note: Floats cause small inaccuracies in certain cases\n");
    test_code_segment_1();
    test_code_segment_2();
    test_code_segment_3();
    test_code_segment_4();
    return 0;
}
