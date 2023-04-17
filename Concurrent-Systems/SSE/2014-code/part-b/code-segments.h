// CSU33014 Exam Solutions, 2014
//
// Question 1 b
//
// DO NOT CHANGE ANY CODE IN THIS FILE
//
// Credit: Alexander Sepelenco, sepelena@tcd.ie
#ifndef CODE_SEGMENTS_H
#define CODE_SEGMENTS_H

// code-segment-1
void shift(float * array, int size);
void vec_shift(float * array, int size); 

// code-segment-2
float compute(float * restrict a, float * restrict b);
float vec_compute(float * restrict a, float * restrict b);

#endif
