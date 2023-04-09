//
// CSU33014 Annual Exam, May 2021
// Question 2
//
// Main file for testing the parallel routine
//
// Please do not change anything in this file

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/time.h>
#include "csu33014-annual-q2-code.h"

// generate a random integer, which may or may not be signed
int get_random(int range, int is_signed) {
  int num = random();
  int sign = random();

  num = num % range;
  // if signed, then 50% should be negative
  if ( is_signed && (sign % 2 == 1) ) {
    num = -num;
  }
  return num;
}

// seed the pseudo-random number generator using the system clock
void initialize_random() {
  struct timeval seedtime;
  int seed;
  
  /* use the microsecond part of the current time as a pseudorandom seed */
  gettimeofday(&seedtime, NULL);
  seed = seedtime.tv_usec;
  srandom(seed);
}

// get the difference in microseconds between two times
long long time_diff(struct timeval start, struct timeval stop) {
  long long result = (stop.tv_sec - start.tv_sec) * 1000000L +
    (stop.tv_usec - start.tv_usec);
  return result;
}


// generate a random string of upper-case characters in the range 'A'
// to 'A' + alphabet_size.
char * gen_random_string(int string_size, int alphabet_size) {
  char * result;

  result = malloc(sizeof(char)*(string_size+1));
  
  // check we're not trying to use more than 26 letters
  assert( alphabet_size <= 26 );
  for ( int i = 0; i < string_size; i++ ) {
    int offset = get_random(alphabet_size, 0);
    result[i] = 'A' + offset;
  }
  // don't forget the null-char terminator
  result[string_size] = '\0';

  return result;
}
  

// create an array containing random strings of upper case chars
char ** gen_random_array_of_strings(int nstrings, int alphabet_size,
				    int string_size) {
  char ** result;

  // allocate the result array
  result = malloc(sizeof(char *) * nstrings);
  assert ( result != NULL );
  
  // fill the result array with random strings
  for ( int i = 0; i < nstrings; i++ ) {
    result[i] = gen_random_string(string_size, alphabet_size);
  }

  return result;
}

// main function to test the code
int main(int argc, char ** argv) {
  int count_warmup, count_correct, count_reduced, count_parallel;
  struct timeval start_time;
  struct timeval stop_time;
  long long compute_time;
  const int alphabet_size = 8;
  const int string_length = 4;

  if ( argc != 2 ) {
    fprintf(stderr, "Usage: ./finddups <nstrings>\n");
    exit(1);
  }

  int nstrings = atoi(argv[1]);
  if ( nstrings < 5 ) {
    fprintf(stderr, "Fatal: Minimum number of strings is 5, not %d\n", nstrings);
    exit(1);
  }
  
  // initialize the pseudo-random number generator
  initialize_random();

  // create inputs and empty output array
  int size_a = nstrings;
  int size_b = nstrings / 2;
  char ** list_a = gen_random_array_of_strings(size_a, alphabet_size, string_length);
  char ** list_b = gen_random_array_of_strings(size_b, alphabet_size, string_length);
  bool * duplicates_correct;
  bool * duplicates_parallel;
  
  // run the student's parallel version of the code
  gettimeofday(&start_time, NULL);
  duplicates_parallel = find_duplicates_parallel(list_a, size_a, list_b, size_b);
  gettimeofday(&stop_time, NULL);
  compute_time = time_diff(start_time, stop_time);
  fprintf(stderr, "Parallel time: %lld, ", compute_time);
  
  // eliminate duplicates with original sequential version of the code
  gettimeofday(&start_time, NULL);
  duplicates_correct = find_duplicates_sequential(list_a, size_a, list_b, size_b);
  gettimeofday(&stop_time, NULL);
  compute_time = time_diff(start_time, stop_time);
  fprintf(stderr, "Sequential time: %lld\n", compute_time);

  // find any difference between the correct and parallel versions
  int bad_match = 0;
  int num_duplicates = 0;
  for ( int i = 0; i < size_a; i++ ) {
    if ( duplicates_correct[i] ) {
      num_duplicates++;
    }
    if ( duplicates_correct[i] != duplicates_parallel[i] ) {
      fprintf(stderr, "  Bad match at %d:", i);
      fprintf(stderr, " correct: %s, ", duplicates_correct[i] ? "true" : "false");
      fprintf(stderr, " parallel: %s\n", duplicates_parallel[i] ? "true" : "false");
      bad_match = 1;
    }
  }
  
  // if no bad match, the parallel version gets the right answer
  if ( bad_match == 0 ) {
    fprintf(stderr, "Success: parallel and sequential versions agree. %d duplicates of %d items\n", num_duplicates, size_a);
  }

  // free all memory allocations from main 
  for (int i = 0; i < size_a; i++) {
    free(list_a[i]);
  }
  for (int i = 0; i < size_b; i++) {
    free(list_b[i]);
  }
  free(list_a);
  free(list_b);
  free(duplicates_correct);
  free(duplicates_parallel);
  return 0;
}
