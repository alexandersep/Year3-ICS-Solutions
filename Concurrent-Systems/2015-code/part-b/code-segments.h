// CSU33014 Exam Solutions, 2015
//
// Question 1 b
//
// DO NOT CHANGE ANY CODE IN THIS FILE
//
// Credit: Alexander Sepelenco, sepelena@tcd.ie
#ifndef CODE_SEGMENTS_H
#define CODE_SEGMENTS_H

// code-segment-1
void mul_neighbour(float * array, int size);
void vec_mul_neighbour(float * array, int size); 

// code-segment-2
float dot_product(float * restrict a, float * restrict b);
float vec_dot_product(float * restrict a, float * restrict b);

// code-segment-3
float rgb_sum_product(float * pixels);
float vec_rgb_sum_product(float * pixels);

// code segment-4
int max_index(float* array);
int vec_max_index(float* array);

#endif
