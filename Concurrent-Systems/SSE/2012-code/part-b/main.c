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

// compute square of dstance between two char arrays
double char_diff_square(char * a, char * b, int size) {
    double sum = 0.0;
    for ( int i = 0; i < size; i++ ) {
        char diff = a[i] - b[i];
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

void test_code_segment_2() {
    const int SIZE = 1027; 
    char* a = malloc(sizeof(float) * SIZE); 
    char* b = malloc(sizeof(float) * SIZE); 
    char* c = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        a[i] = 'A' + (rand() % 26);
    }
    memcpy_2(b, a, SIZE);
    vec_memcpy_2(c, a, SIZE);

    double diff = char_diff_square(b, c, SIZE); 
    fprintf(stderr, "memcpy_2 diff: %lf\n", diff);

    free(a);
    free(b);
    free(c);
}

void test_code_segment_3() {
    const int SIZE = 128; 
    float* a = malloc(sizeof(float) * SIZE); 
    float* b = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
        b[i] = i;
    }
    float dp = dot_product(a, b, SIZE/4);
    float vdp = vec_dot_product(a, b, SIZE/4);

    double diff = dp - vdp; 
    fprintf(stderr, "dot_product diff: %lf\n", diff);

    free(a);
    free(b);
}

void test_code_segment_4() {
    const int SIZE = 1027; 
    float* array = malloc(sizeof(float) * SIZE); 
    float mean = 0;
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        array[i] = (float)(rand() % SIZE);
        mean += array[i];
    }
    mean /= SIZE;
    int std = stddev(mean, array, SIZE);
    int vstd = vec_stddev(mean, array, SIZE);

    double diff = std - vstd; 
    fprintf(stderr, "stddev  diff: %lf\n", diff);

    free(array);
}

int main() {
    srand(time(NULL));
    printf("Note: Floats cause small inaccuracies in certain cases\n");
    test_code_segment_2();
    test_code_segment_3();
    test_code_segment_4();
    return 0;
}
