// CSU33014 Exam Solutions, 2012
//
// Question 1 b
//
// DO NOT CHANGE ANY CODE IN THIS FILE
//
// Credit: Alexander Sepelenco, sepelena@tcd.ie
#ifndef CODE_SEGMENTS_H
#define CODE_SEGMENTS_H

// code-segment-1
void add_scaled(float * a, float * b, float * c, float factor);
void vec_add_scaled(float * a, float * b, float * c, float factor);

// code-segment-2
void memcpy_2(char * restrict dest, char * restrict src, int size); 
void vec_memcpy_2(char * restrict dest, char * restrict src, int size); 

// code-segment-3
float dot_product(float * a, float * b, int s); 
float vec_dot_product(float * a, float * b, int s); 

// code segment-4
float stddev(float mean, float * samples, int size); 
float vec_stddev(float mean, float * samples, int size); 

#endif
