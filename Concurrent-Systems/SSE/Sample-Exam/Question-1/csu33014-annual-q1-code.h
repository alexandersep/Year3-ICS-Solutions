// CSU33014 Annual Exam, May 2022
// Question 1
//
// For instructions see csu33014-annual-q1-code.c
//
// PLEASE DO NOT CHANGE ANY CODE IN THIS FILE

#ifndef csu33014_annual_q1_code_H
#define csu33014_annual_q1_code_H

void Q1_routine_0(float * restrict a, float * restrict b, float * restrict c);
void Q1_vectorized_0(float * restrict a, float * restrict b, float * restrict c);

float Q1_routine_1(float * restrict a, float * restrict b, int size);
float Q1_vectorized_1(float * restrict a, float * restrict b, int size);

void Q1_routine_2(float * restrict a, float * restrict b, int size);
void Q1_vectorized_2(float * restrict a, float * restrict b, int size);

void Q1_routine_3(float * restrict a, float * restrict b, int size);
void Q1_vectorized_3(float * restrict a, float * restrict b, int size);

void Q1_routine_4(float * restrict a, float * restrict b, float * restrict c);
void Q1_vectorized_4(float * restrict a, float * restrict b, float * restrict c);

void Q1_routine_5(float * restrict a, float * restrict b,
		  float * restrict c, int size);
void Q1_vectorized_5(float * restrict a, float * restrict b,
                     float * restrict c, int size);

float Q1_routine_6(float * restrict a, int size);
float Q1_vectorized_6(float * restrict a, int size);

#endif
