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

void test_code_segment_2() {
    const int SIZE = 4096; 
    float* a = _mm_malloc(sizeof(float) * SIZE, 16); 
    float* b = _mm_malloc(sizeof(float) * SIZE, 16); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        a[i] = i+1;
        b[i] = a[i]; 
    }
    code_segment_2(a);
    vec_code_segment_2(b);

    double diff = diff_square(a, b, SIZE);
    fprintf(stderr, "code_segment_2 diff: %lf\n", diff);

    _mm_free(a);
    _mm_free(b);
}

int main() {
    srand(time(NULL));
    printf("Note: Floats cause small inaccuracies in certain cases\n");
    test_code_segment_2();
    return 0;
}
