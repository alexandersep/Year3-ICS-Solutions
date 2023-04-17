// CSU33014 Exam Solutions, 2017
//
// Question 1 b
//
// DO NOT CHANGE ANY CODE IN THIS FILE
//
// Credit: Alexander Sepelenco, sepelena@tcd.ie
#ifndef CODE_SEGMENTS_H
#define CODE_SEGMENTS_H

// code-segment-1
void compute(float * a, float * b);
void vec_compute(float * a, float * b); 

// code-segment-2
float find_max(float * a, int size);
float vec_find_max(float * a, int size);

// code-segment-3
float rgb_sum_product(float * pixels);
float vec_rgb_sum_product(float * pixels);

// code segment-4
int multiply(float ** restrict matrix, float * restrict vec, float * restrict result);
int vec_multiply(float ** restrict matrix, float * restrict vec, float * restrict result);

#endif
