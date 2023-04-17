#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <time.h>
#include <math.h>
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

// compare each value with fabs() function from <math.h>
// Used for when difference of squares is problematic
void compare(float * a, float * b, int size, char* name_of_segment) {
    int i;
    int is_correct = 1;
    for (i = 0; i < size; i++) {
        if (fabs(a[i] - b[i]) > 0.0000001) {
            fprintf(stderr, "arrays differ: a[%d]: %f, b[%d]: %f\n", i, a[i], i, b[i]);
            is_correct = 0;
        }
    }
    if (is_correct) {
        fprintf(stderr, "%s is probably correct\n", name_of_segment);
    } else {
        fprintf(stderr, "%s is incorrect\n", name_of_segment);
    }
}

void test_code_segment_1() {
    const int SIZE = 1000; 
    float* fibs = _mm_malloc(sizeof(float) * SIZE, 16); 
    float* clone_fibs = _mm_malloc(sizeof(float) * SIZE, 16); 
    code_segment_1(fibs);
    vec_code_segment_1(clone_fibs);

    compare(fibs, clone_fibs, SIZE, "code_segment_1");

    _mm_free(fibs);
    _mm_free(clone_fibs);
}

void test_code_segment_2() {
    const int SIZE = 1027; 
    float* a = _mm_malloc(sizeof(float) * SIZE, 16); 
    float* b = _mm_malloc(sizeof(float) * SIZE, 16); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
        b[i] = i; 
    }
    code_segment_2(a);
    vec_code_segment_2(b);

    double diff = diff_square(a, b, SIZE);
    fprintf(stderr, "code_segment_2 diff: %lf\n", diff);

    _mm_free(a);
    _mm_free(b);
}

void test_code_segment_3() {
    const int SIZE = 1024; 
    float* vals = _mm_malloc(sizeof(float) * SIZE, 16); 
    float* clone_vals = _mm_malloc(sizeof(float) * SIZE, 16); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        vals[i] = i;
        clone_vals[i] = i;
    }
    code_segment_3(vals);
    vec_code_segment_3(clone_vals);

    double diff = diff_square(vals, clone_vals, SIZE);
    fprintf(stderr, "code_segment_3 diff: %lf\n", diff);

    _mm_free(vals);
    _mm_free(clone_vals);
}

void test_code_segment_4() {
    const int SIZE = 1027; 
    const float factor = 1.5;
    float* vals = malloc(sizeof(float) * SIZE); 
    float* clone_vals = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        vals[i] = i; 
        clone_vals[i] = vals[i];
    }
    code_segment_4(vals, factor, SIZE);
    vec_code_segment_4(clone_vals, factor, SIZE);
    compare(vals, clone_vals, SIZE, "code_segment_4");

    free(vals);
    free(clone_vals);
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
