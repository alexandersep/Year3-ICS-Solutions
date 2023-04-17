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
    const float factor = 1.5f;
    float* a = malloc(sizeof(float) * SIZE); 
    float* b = malloc(sizeof(float) * SIZE); 
    float* c = malloc(sizeof(float) * SIZE); 
    float* d = malloc(sizeof(float) * SIZE); 
    float* e = malloc(sizeof(float) * SIZE); 
    float* f = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        b[i] = i;
        c[i] = i;
        e[i] = i;
        f[i] = i;
    }
    add_scaled(a, b, c, factor);
    vec_add_scaled(d, e, f, factor);

    double diff = diff_square(a, d, SIZE); 
    fprintf(stderr, "add_scaled diff: %lf\n", diff);

    free(a);
    free(b);
    free(c);
    free(d);
    free(e);
    free(f);
}

void test_code_segment_2() {
    const int SIZE = 1234; 
    float* a = malloc(sizeof(float) * SIZE); 
    float* b = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
        b[i] = i; 
    }
    float dp = dot_product(a, b, SIZE);
    float vdp = vec_dot_product(a, b, SIZE);
    double diff = dp - vdp; 
    fprintf(stderr, "dot_product diff: %lf\n", diff);
    free(a);
    free(b);
}

int main() {
    srand(time(NULL));
    printf("Note: Floats cause small inaccuracies in certain cases\n");
    test_code_segment_1();
    test_code_segment_2();
    return 0;
}
