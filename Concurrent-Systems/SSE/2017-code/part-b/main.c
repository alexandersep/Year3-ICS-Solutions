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
    const int SIZE = 1024; 
    const float multiplier = 1.5f;
    float* a = malloc(sizeof(float) * SIZE); 
    float* b = malloc(sizeof(float) * SIZE); 
    float* c = malloc(sizeof(float) * SIZE);
    // set values for testing
    const float scalar = 10234.343;
    for (int i = 0; i < SIZE; i++) {
        a[i] = i+scalar;
    }
    compute(a, b);
    vec_compute(a, c);
    double diff = diff_square(b, c, SIZE); 
    fprintf(stderr, "compute  diff: %lf\n", diff);

    free(a);
    free(b);
    free(c);
}

void test_code_segment_2() {
    const int SIZE = 1025; 
    const float multiplier = 1.5f;
    float* array = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        array[i] = rand();
    }
    float fm = find_max(array,SIZE);
    float vfm = vec_find_max(array,SIZE);

    double diff = fm - vfm;
    fprintf(stderr, "find_max diff: %lf\n", diff);

    free(array);
}

void test_code_segment_3() {
    const int SIZE = 1011; 
    float* pixels = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        pixels[i] = i*100;
    }
    float * clone_pixels = clone_float_array(pixels, SIZE);

    float rgbp = rgb_sum_product(pixels);
    float vrgbp = vec_rgb_sum_product(clone_pixels);

    double diff = rgbp - vrgbp;
    fprintf(stderr, "rgb_sum_product diff: %lf\n", diff);

    free(pixels);
    free(clone_pixels);
}

void test_code_segment_4() {
    const int SIZE = 4096;
    float** restrict matrix = malloc(sizeof(float*) * SIZE);
    float* restrict vec = malloc(sizeof(float) * SIZE);
    float* restrict result = malloc(sizeof(float) * SIZE);
    float* restrict vec_result = malloc(sizeof(float) * SIZE);
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        matrix[i] = malloc(sizeof(float) * SIZE);
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = 6; 
        }
        vec[i] = 5; 
    }
    multiply(matrix, vec, result);
    vec_multiply(matrix, vec, vec_result);

    double diff = diff_square(result, vec_result, SIZE); 
    fprintf(stderr, "multiply diff: %lf\n", diff);

    for (int i = 0; i < SIZE; i++) {
        free(matrix[i]); 
    }
    free(matrix);
    free(vec);
    free(result);
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
