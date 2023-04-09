// CSU33014 Annual Exam, May 2022
// Question 1
//
// For instructions see csu33014-annual-q1-code.c
//
// PLEASE DO NOT CHANGE ANY CODE IN THIS FILE

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include "csu33014-annual-q1-code.h"

/* test code for the routines to be vectorized */

// seed the pseudo-random number generator using the system clock
void initialize_random() {
  struct timeval seedtime;
  int seed;
  
  /* use the microsecond part of the current time as a pseudorandom seed */
  gettimeofday(&seedtime, NULL);
  seed = seedtime.tv_usec;
  fprintf(stderr, "seed %d\n", seed);
  srandom(seed);
}

// generate a random integer, which may be positive (>0) or signed
int get_random(int range, int is_positive) {
  int num = random();
  int sign = random();

  num = num % range;
  // if positive, then ensure no zero values
  if ( is_positive ) {
    if ( num == 0 ) {
      num = 1;
    }
    assert( num >= 1 );
  }
  // otherwise create a signed value that may be zero or negative
  else if ( sign % 2 == 1 ) {
    num = -num;
  }
  return num;
}

// create a new array of float with random values
float * new_random_float_array(int size, int is_positive, int spec_range) {
  int range;

  if ( spec_range > 0 ) {
    range = spec_range;
  }
  else {
    range = 256;
  }
  
  float * out = malloc(sizeof(float) * size);

  for ( int i = 0; i < size; i++ ) {
    out[i] = get_random(range, is_positive);
  }
  return out;
}

// create a new array of characters with random values
unsigned char * new_random_char_array(int size) {
  int range = 256;
  int is_positive = 1;
  unsigned char * out = malloc(sizeof(float) * size);

  for ( int i = 0; i < size; i++ ) {
    out[i] = get_random(range, is_positive);
  }
  return out;
}

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

// generate four arrays to use in testing
void gen_test_arrays(float ** out_correct, float ** out_vectorized,
		     float ** b, float **c, int size) {
  *out_correct = malloc(sizeof(float) * size);
  *out_vectorized = malloc(sizeof(float) * size);
  *b = new_random_float_array(size, 1, 0);
  *c = new_random_float_array(size, 1, 0);
}

void free_arrays(float * x, float *y, float *z, float * w) {
  free(x);
  free(y);
  free(z);
  free(w);
}

// simple test of routine 0
void test_routine_0() {
  int size = 1024;
  float * out_correct, * out_vectorized, *b, *c;
  double diff;

  gen_test_arrays(&out_correct, &out_vectorized, &b, &c, size);

  Q1_routine_0(out_correct, b, c);
  Q1_vectorized_0(out_vectorized, b, c);

  diff = diff_square(out_correct, out_vectorized, size);

  free_arrays(out_correct, out_vectorized, b, c);
  
  printf("routine_0 diff: %lf\n", diff);
}

// simple test of routine 1
void test_routine_1() {
  int size = get_random(5, 1) + 10; // get a random size > 10
  float * a = new_random_float_array(size, 1, 16);
  float * b = new_random_float_array(size, 1, 16);
  float out_correct, out_vectorized;
  double diff;

  out_vectorized = Q1_vectorized_1(a, b, size);
  out_correct = Q1_routine_1(a, b, size);
  //fprintf(stderr, "%f %f\n", out_correct, out_vectorized);
  
  free(a);
  free(b);
  
  diff = out_correct - out_vectorized;

  fprintf(stderr, "routine1 diff: %lf\n", diff);
}

// create a copy of an existing array
float * clone_float_array(float * src, int size) {
  float * dest = malloc(sizeof(float)*size);
  for ( int i = 0; i < size; i++ ) {
    dest[i] = src[i];
  }
  return dest;
}

// simple test of routine 2
void test_routine_2() {
  int size = get_random(2048, 1) + 10; // get a random size >= 10
  float * out_correct = new_random_float_array(size, 1, 0);
  float * b = new_random_float_array(size, 1, 0);
  float * out_vectorized = clone_float_array(out_correct, size);
  double diff;

  Q1_routine_2(out_correct, b, size);
  Q1_vectorized_2(out_vectorized, b, size);

  diff = diff_square(out_correct, out_vectorized, size);

  free(out_correct);
  free(out_vectorized);
 
  fprintf(stderr, "routine2 diff: %lf\n", diff);
}




// simple test of routine 3
void test_routine_3() {
  int size = get_random(2048, 1) + 10; // get a random size > 10
  float * out_correct = new_random_float_array(size, 1, 0);
  float * out_vectorized = clone_float_array(out_correct, size);
  float * b = new_random_float_array(size, 1, 0);
  double diff;

  // fprintf(stderr, "out_correct %p, out_vectorized %p, b %p, size %d\n", out_correct, out_vectorized, b, size);

  Q1_routine_3(out_correct, b, size);
  Q1_vectorized_3(out_vectorized, b, size);

  diff = diff_square(out_correct, out_vectorized, size);

  free(out_correct);
  free(out_vectorized);
  free(b);
  
  fprintf(stderr, "routine3 diff: %lf\n", diff);
}

// simple test of routine 4
void test_routine_4() {
  int size = 2048;
  float * out_correct, * out_vectorized, *b, *c;
  double diff;
  
  gen_test_arrays(&out_correct, &out_vectorized, &b, &c, size);

  Q1_routine_4(out_correct, b, c);
  Q1_vectorized_4(out_vectorized, b, c);
  
  diff = diff_square(out_correct, out_vectorized, size);
  
  free_arrays(out_correct, out_vectorized, b, c);
  
  fprintf(stderr, "routine4 diff: %lf\n", diff);
}


// simple test of routine 5
void test_routine_5() {
  int size = 2048;
  float * out_correct, * out_vectorized, *b, *c;
  double diff;
  
  gen_test_arrays(&out_correct, &out_vectorized, &b, &c, size);
  out_vectorized = clone_float_array(out_correct, size);
  float * b2 = clone_float_array(b, size);
  float * c2 = clone_float_array(c, size);

  Q1_routine_5(out_correct, b, c, size);
  Q1_vectorized_5(out_vectorized, b2, c2, size);
  
  diff = diff_square(out_correct, out_vectorized, size);
  
  free_arrays(out_correct, out_vectorized, b, c);
  
  fprintf(stderr, "routine5 diff: %lf\n", diff);
}

#if 0
// simple test of routine 5
void test_routine_5() {
  int size = get_random(2048, 1) + 10; // get a random size > 10
  unsigned char * b = new_random_char_array(size);
  unsigned char * a;
  int diff = get_random(2, 0);
  int result_correct, result_vectorized, match;

  if ( diff == 0 ) {
    a = b;
  }
  else {
    a = new_random_char_array(size);
  }
  
  result_correct = routine_5(a, b, size);
  result_vectorized = vectorized_5(a, b, size);
  match = result_correct == result_vectorized;

  fprintf(stderr, "routine_5 correct (1 is true): %d\n", match);
}
#endif

void test_routine_6() {
  int size = get_random(16, 1) + 10; // get a random size > 10
  float * a = new_random_float_array(size, 1, 0);
   float out_correct, out_vectorized;
  double diff;

  out_vectorized = Q1_vectorized_6(a, size);
  out_correct = Q1_routine_6(a, size);
  //fprintf(stderr, "%f %f\n", out_correct, out_vectorized);
  
  free(a);
  
  diff = out_correct - out_vectorized;

  fprintf(stderr, "routine6 diff: %lf\n", diff);
}

#if 0
void test_routine_6() {
  int size = 1023;
  float * out_correct, * out_vectorized, *b, *c;
  double diff;

  gen_test_arrays(&out_correct, &out_vectorized, &b, &c, size);

  routine_6(out_correct, b, c);
  vectorized_6(out_vectorized, b, c);

  diff = diff_square(out_correct, out_vectorized, size);

  free_arrays(out_correct, out_vectorized, b, c);
  
  fprintf(stderr, "routine6 diff: %lf\n", diff);
}
#endif


// simple main function containing code to test each
// 
int main() {
  initialize_random();
  test_routine_0();
  test_routine_1();
  test_routine_2();
  test_routine_3();
  test_routine_4();
  test_routine_5();
  test_routine_6(); 
  
  return 0;
}
