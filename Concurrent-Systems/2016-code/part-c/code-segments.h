// CSU33014 Exam Solutions, 2016
//
// Question 1 c
//
// DO NOT CHANGE ANY CODE IN THIS FILE
//
// Credit: Alexander Sepelenco, sepelena@tcd.ie
#ifndef CODE_SEGMENTS_H
#define CODE_SEGMENTS_H

// code-segment-1
void add_scaled(float* a, float* b, float* c, float factor);
void vec_add_scaled(float* a, float* b, float* c, float factor);

// code-segment-2
float dot_product(float * restrict a, float * restrict b, int size);
float vec_dot_product(float * restrict a, float * restrict b, int size);

#endif
