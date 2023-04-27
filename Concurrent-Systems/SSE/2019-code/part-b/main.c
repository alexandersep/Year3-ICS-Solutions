#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <time.h>
#include "code-segments.h"

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
    float * a = malloc(sizeof(float) * SIZE);
    for (int i = 0; i < SIZE; i++) {
        a[i] = rand() % SIZE;
    }
    float * clone_a = clone_float_array(a, SIZE);

    float sum = sums(a, SIZE);
    float vec_sum = vec_sums(clone_a, SIZE);

    double diff = sum - vec_sum;
    fprintf(stderr, "sum diff: %lf\n", diff);

    free(a);
    free(clone_a);
}

int main() {
    srand(time(NULL));
    printf("Note: Floats cause small inaccuracies in certain cases\n");
    test_code_segment_2();
    return 0;
}
