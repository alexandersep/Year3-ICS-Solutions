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

// create a copy of an existing array
float * clone_float_array(float * src, int size) {
    float * dest = malloc(sizeof(float)*size);
    for ( int i = 0; i < size; i++ ) {
        dest[i] = src[i];
    }
    return dest;
}

void test_code_segment_1() {
    const int SIZE = 1027; 
    const float factor = 1.5;
    float * a = malloc(sizeof(float) * SIZE); 
    float * b = malloc(sizeof(float) * SIZE); 
    float * c = malloc(sizeof(float) * SIZE); 
    float * d = malloc(sizeof(float) * SIZE); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        b[i] = rand() % SIZE; 
        c[i] = rand() % SIZE; 
    }
    float * e = clone_float_array(b, SIZE);
    float * f = clone_float_array(c, SIZE);
    do_calc(a, b, c, factor, SIZE);
    vec_do_calc(d, e, f, factor, SIZE);

    double diff = diff_square(a, d, SIZE); 
    fprintf(stderr, "do_calc diff: %lf\n", diff);

    free(a);
    free(b);
    free(c);
    free(d);
    free(e);
    free(f);
}

void test_code_segment_2() {
    const int SIZE = 1024; 
    const float num = 3;
    float* a = malloc(sizeof(float) * SIZE); 
    float* b = malloc(sizeof(float) * SIZE); 
    first_1024_powers(num, a);
    vec_first_1024_powers(num, b);

    fprintf(stderr, "first_1024_power (not quite testable)\n");
    //compare(a, b, SIZE, "fire_1024_power"); // uncomment to see if you're roughly in the range

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
