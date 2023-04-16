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

void test_code_segment_2() {
    const int SIZE = 1027; 
    const float multiplier = 1.5f;
    float * array = malloc(sizeof(float) * SIZE);
    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() % SIZE;
    }
    float * clone_array = clone_float_array(array, SIZE);

    compute_2(array, SIZE, multiplier);
    vec_compute_2(clone_array, SIZE, multiplier);

    double diff = diff_square(array, clone_array, SIZE); 
    fprintf(stderr, "compute_2 diff: %lf\n", diff);

    free(array);
    free(clone_array);
}

void test_code_segment_3() {
    const int SIZE = 2049; 
    int* array = _mm_malloc(sizeof(int) * SIZE, 16); // align by 16 bytes 
    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() % SIZE;
    }
    int c3 = compute_3(array,SIZE);
    int vc3 = vec_compute_3(array,SIZE);

    double diff = c3 - vc3; 
    fprintf(stderr, "compute_3  diff: %lf\n", diff);

    _mm_free(array);
}

void test_code_segment_4() {
    const int strlength = 1023; 

    char* string1 = malloc(sizeof(char) * strlength);
    char* string2 = malloc(sizeof(char) * strlength);
    for (int i = 0; i < strlength; i++) {
        string1[i] = 'A' + (rand() % 26);
        string2[i] = string1[i]; // change this to string1[i] - 1 to see if not matching 
    }
    bool s = strcmp_4(string1,string2,strlength);
    bool vs = vec_strcmp_4(string1,string2,strlength);

    (s == vs) ? 
        fprintf(stderr, "compute_4 correct: s = %d, vs = %d\n", s, vs) : 
        fprintf(stderr, "compute_4 incorrect: s = %d, vs = %d\n", vs, vs);

    free(string1);
    free(string2);
}

int main() {
    srand(time(NULL));
    printf("Note: Floats cause small inaccuracies in certain cases\n");
    test_code_segment_2();
    test_code_segment_3();
    test_code_segment_4();
    return 0;
}
