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
    const int SIZE = 1048; 
    float* a = _mm_malloc(sizeof(float) * SIZE, 16); 
    float* b = _mm_malloc(sizeof(float) * SIZE, 16); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
        b[i] = i;
    }

    code_segment_1(a);
    vec_code_segment_1(b);

    double diff = diff_square(a, b, SIZE);
    fprintf(stderr, "code_segment_1 diff: %lf\n", diff);

    _mm_free(a);
    _mm_free(b);
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
    const int SIZE = 256; 
    const int number = 2; 
    float* pow = _mm_malloc(sizeof(float) * SIZE, 16); 
    float* clone_pow = _mm_malloc(sizeof(float) * SIZE, 16); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        pow[i] = i;
        clone_pow[i] = i;
    }

    code_segment_3(number, pow);
    vec_code_segment_3(number, clone_pow);

    compare(pow, clone_pow, SIZE, "code_segment_4");

    _mm_free(pow);
    _mm_free(clone_pow);
}

void test_code_segment_4() {
    const int SIZE = 1000; 
    float* numbers = _mm_malloc(sizeof(float) * SIZE, 16); 
    // set values for testing
    for (int i = 0; i < SIZE; i++) {
        numbers[i] = rand() % (int) (SIZE / 500); 
    }
    float mi = code_segment_4(numbers);
    float vmi = vec_code_segment_4(numbers);

    double diff = mi - vmi;
    fprintf(stderr, "code_segment_4  diff: %lf\n", diff);

    _mm_free(numbers);
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
