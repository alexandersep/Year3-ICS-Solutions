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
    const int SIZE = 1029; 
    float* a = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
    }
    float* b = clone_float_array(a, SIZE);

    shift(a, SIZE);
    vec_shift(b,SIZE);

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
    float* c = clone_float_array(a, SIZE);
    float* d = clone_float_array(b, SIZE);
    float dp = compute(a, b);
    float vdp = vec_compute(c, d);

    double diff = dp - vdp; 
    fprintf(stderr, "dot_product diff: %lf\n", diff);

    free(a);
    free(b);
    free(c);
    free(d);
}

int main() {
    srand(time(NULL));
    printf("Note: Floats cause small inaccuracies in certain cases\n");
    test_code_segment_1();
    test_code_segment_2();
    return 0;
}
