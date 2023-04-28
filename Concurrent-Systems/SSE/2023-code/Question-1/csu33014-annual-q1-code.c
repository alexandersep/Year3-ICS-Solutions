//
// CSU33014 Annual Exam, April 2023
// Question 1
//

// Please examine version each of the following routines with names
// starting 'routine_'. Where the routine can be vectorized, please
// replace the corresponding 'vectorized' version using SSE vector
// intrinsics. Where it cannot be vectorized please explain why.

// To illustrate what you need to do, routine_0 contains a
// non-vectorized piece of code, and vectorized_0 shows a
// corresponding vectorized version of the same code.

// Note that to simplify testing, I have put a copy of the original
// non-vectorized code in the vectorized version of the code for
// routines 1 to 6. This allows you to easily see what the output of
// the program looks like when the original and vectorized version of
// the code produce equivalent output.

// Note the restrict qualifier in C indicates that "only the pointer
// itself or a value directly derived from it (such as pointer + 1)
// will be used to access the object to which it points".


#include <immintrin.h>
#include <stdio.h>
#include <xmmintrin.h>

#include "csu33014-annual-q1-code.h"


/****************  routine 0 *******************/

// Here is an example routine that should be vectorized
void Q1_routine_0(float * restrict a, float * restrict b, float * restrict c) {
  for (int i = 0; i < 1024; i++ ) {
    a[i] = b[i] * c[i];
  }
}

// here is a vectorized solution for the example above
void Q1_vectorized_0(float * restrict a, float * restrict b, float * restrict c) {
  __m128 a4, b4, c4;
  
  for (int i = 0; i < 1024; i = i+4 ) {
    b4 = _mm_loadu_ps(&b[i]);
    c4 = _mm_loadu_ps(&c[i]);
    a4 = _mm_mul_ps(b4, c4);
    _mm_storeu_ps(&a[i], a4);
  }
}

/***************** routine 1 *********************/

// in the following, size can have any positive value
void Q1_routine_1(float * restrict a, float * restrict b, int size) { 
  for ( int i = 0; i < size; i++ ) {
    a[i] = 3.73892 + (b[i] * 3.73892); 
  }
}

// in the following, size can have any positive value
void Q1_vectorized_1(float * restrict a, float * restrict b, int size) {
    int i; // save i variable for entire function and postloop
    int size_minus_three = size - 3;  // optimise comparision for "for loop" below
    __m128 vf32_const = _mm_set1_ps(3.73892); // set a vector with all lanes as constant
    for ( int i = 0; i < size_minus_three; i+=4) { // iterate over 4 at a time i < size - 3
        __m128 vf32_b = _mm_loadu_ps(&b[i]); // load array "b" at index i, 4 values unaligned
        __m128 vf32_mul = _mm_mul_ps(vf32_b, vf32_const); // multiply vectors const and b
        __m128 vf32_add = _mm_add_ps(vf32_const, vf32_mul); // add vectors mul and const
        _mm_storeu_ps(&a[i], vf32_add); // store unaligned answer of addition to 4 values of array "a"
    }
    for (/* i = i */; i < size; i++ ) { // postloop if not(i < size - 3)
        a[i] = 3.73892 + (b[i] * 3.73892); 
    }
}


/******************* routine 2 ***********************/

// in the following, size can have any positive value
float Q1_routine_2(float * restrict a, int size) {
  float x = 0.0;
  float y = 0.0;

  for ( int i = 0; i < size-1; i++ ) {
    x = x + a[i];
    y = y + a[i+1];
  }
  return x + y * 10.0;
}

// Source: https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction
// Fast horizonal multiplication using SSE3
// Equivalent to two hadds's but with multiplying and a return lowest lane for float vectors
/* Equivalent in code
vector = _mm_hadd_ps(vector, vector);
vector = _mm_hadd_ps(vector, vector);
float x = _mm_cvtss_f32(vector); // extract lower 32 bits = [10,x,x,x]
*/
static inline float hsum_ps_sse3(__m128 vf32_a) { // vf32_a = [1,2,3,4]
    __m128 shuf = _mm_movehdup_ps(vf32_a); // shuf = [2,2,4,4]
    __m128 sums = _mm_add_ps(vf32_a, shuf); // sums = vf32_a + shuf = [3,4,7,8]
    shuf        = _mm_movehl_ps(shuf, sums); // shuf = [7,8,4,4]
    sums        = _mm_add_ss(sums, shuf); // sums + shuf lower = [10,x,x,x]
    return _mm_cvtss_f32(sums); // extract lower 32 bits = [10,x,x,x]
}

// in the following, size can have any positive value
float Q1_vectorized_2(float * restrict a, int size) {
    int i;
    __m128 vf32_x = _mm_setzero_ps(); // fill lanes with [0,0,0,0] 
    __m128 vf32_y = _mm_setzero_ps(); // fill lanes with [0,0,0,0] 
    int size_minus_three = size-3; // optimized for "for loop" computation
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_tempx = _mm_loadu_ps(&a[i]); // load 4 values of array "a", [a[i], a[i+1], a[i+2], a[i+3]]
        __m128 vf32_tempy = _mm_loadu_ps(&a[i+1]); // load 4 values of array "a", [a[i], a[i+1], a[i+2], a[i+3]]
        vf32_x = _mm_add_ps(vf32_tempx, vf32_x); // add vector together vertically so sum is [x1, y1, x2, y2] 
        vf32_y = _mm_add_ps(vf32_tempy, vf32_y); // add vector together vertically so sum is [x1, y1, x2, y2] 
    }
    // [a,b,c,d], 0,1,2,3
    float x = hsum_ps_sse3(vf32_x);
    float y = hsum_ps_sse3(vf32_y);
    for (/* i = i */; i < size-1; i++) { // postloop if not(i < size - 3)
        x = x + a[i];
        y = y + a[i+1];
    }
    return x + y * 10.0; // return final result
}

/******************** routine 3 ************************/

// in the following, size can have any positive value
void Q1_routine_3(float * restrict a, float * restrict b, int size) {
  for ( int i = 0; i < size; i++ ) {
    if ( a[i] > (b[i] + 1.0) ) {
      float temp = a[i];
      a[i] = b[i];
      b[i] = temp;
    }
  }
}

// in the following, size can have any positive value
void Q1_vectorized_3(float * restrict a, float * restrict b, int size) {
    int i;
    int size_minus_three = size - 3; // optimised for "for loop"
    __m128 vf32_one = _mm_set1_ps(1);
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_a = _mm_loadu_ps(&a[i]); // load unaligned 4 bytes of memory to vector
        __m128 vf32_b = _mm_loadu_ps(&b[i]); // load unaligned 4 bytes of memory to vector
        __m128 vf32_b_plus_one = _mm_add_ps(vf32_b, vf32_one); // add 1 to all lanes of vector b
        // if vectored (a[i] > (b[i] + 1.0) swap
        __m128 vf32_cmpgt = _mm_cmpgt_ps(vf32_a, vf32_b_plus_one);

        __m128 vf32_cmpgt_1 = _mm_andnot_ps(vf32_cmpgt, vf32_a);
        __m128 vf32_cmpgt_2 = _mm_and_ps(vf32_cmpgt, vf32_b);
        __m128 vf32_a_a = _mm_or_ps(vf32_cmpgt_1, vf32_cmpgt_2);

        vf32_cmpgt_1 = _mm_andnot_ps(vf32_cmpgt, vf32_b);
        vf32_cmpgt_2 = _mm_and_ps(vf32_cmpgt, vf32_a);
        __m128 vf32_b_b = _mm_or_ps(vf32_cmpgt_1, vf32_cmpgt_2);

        _mm_storeu_ps(&a[i], vf32_a_a); // store unaligned answer back to four values of array "a"
        _mm_storeu_ps(&b[i], vf32_b_b); // store unaligned answer back to four values of array "a"
    }
    for (/* i = i */; i < size; i++ ) { // postloop if not(i < size - 3)
        if ( a[i] > (b[i] + 1.0) ) {
            float temp = a[i];
            a[i] = b[i];
            b[i] = temp;
        }
    }
}


/********************* routine 4 ***********************/

void Q1_routine_4(float * restrict a) {
  for ( int i = 0; i < 2048; i = i + 2 ) {
    float temp = a[i];
    a[i] = a[i+1];
    a[i+1] = temp;
  }
}

void Q1_vectorized_4(float * a) {
    for (int i = 0; i < 2048; i += 4) { // increment by four each time
        __m128 vf32_a = _mm_loadu_ps(&a[i]); // load unaligned float 4 values to vector
        vf32_a = _mm_shuffle_ps(vf32_a, vf32_a, _MM_SHUFFLE(2,3,0,1)); // swaps [a,b,c,d] 0,1,2,3 => [b,a,d,c] 1,0,3,2
        _mm_storeu_ps(&a[i], vf32_a); // stores swaps back into vector four values
    }
}


/********************* routine 5 ***********************/

// in the following, size can have any positive value
void Q1_routine_5(unsigned char * restrict a, unsigned char * restrict b,
                  unsigned char * restrict c, int size) {
  for ( int i = 1; i < size; i++ ) {
    a[i-1] = b[i+1] ^ c[i-1];
  }
}

// in the following, size can have any positive value
void Q1_vectorized_5(unsigned char * restrict a, unsigned char * restrict b, unsigned char * restrict c, int size) {
    int i;
    int size_minus_three = size - 15; // optimise for "for loop" 
    for (i = 1; i < size_minus_three; i+=16) { // increment by 16 as 16 characters = 32 bytes, same as 4 floats bit-wise
        __m128 vf32_c_minus_one = _mm_loadu_ps((float *)&c[i-1]); // load characters (16) as floats for "c[i-1]"
        __m128 vf32_b_plus_one = _mm_loadu_ps((float *)&b[i+1]); // load characters (16) as floats for "b[i+1]"
        _mm_storeu_ps((float *)&a[i-1], _mm_xor_ps(vf32_c_minus_one, vf32_b_plus_one)); // store vector xor of c[i-1], b[i+1] back to a[i-1]
    }
    for (/* i = i */; i < size; i++) { // postloop if i is not divisible by size - 15 in general loop
        a[i-1] = b[i+1] ^ c[i-1];
    }
}
  

/********************* routine 6 ***********************/

// Code fixed as emailed by David Gregg
// in the following size is a positive value that is a multiple of 3
void Q1_routine_6(float * restrict a, float * restrict b, float * restrict c) {
    for ( int i = 0; i < 1024; i++ ) {
        float x = b[i];
        float y = c[i];
        a[i] = 0;
        for ( int j = 0; j < 100; j++ ) {
            x = (x * x) - (y * y);
            y = x * y * 2;
            if ( (x > 1000.0) || (y > 1000.0) ) {
                a[i] = 1;
                break;
            }
        }
    }
}

// This function is incorrect, This will not be fix, fix it yourself
// in the following size is a positive value that is a multiple of 3
void Q1_vectorized_6(float * restrict a, float * restrict b, float * restrict c) {
    __m128 vf32_const_2 = _mm_set1_ps(2); // constant for (y * 2) part
    __m128 vf32_const_1000 = _mm_set1_ps(1000); // constant for (y * 2) part
    __m128 vf32_const_0 = _mm_setzero_ps(); // constant for a[i] = 0
    __m128 vf32_const_1 = _mm_set1_ps(1); // constant one vector
    for (int i = 0; i < 1024; i+=4) { // increment by four each time, postloop not requires as 1024 % 4 == 0
        __m128 vf32_x = _mm_loadu_ps(&b[i]); // load 4 values from float b
        __m128 vf32_y = _mm_loadu_ps(&c[i]); // load 4 values from float c
        _mm_storeu_ps(&a[i], vf32_const_0);

        for (int j = 0; j < 100; j+=4) {
            // optimise out inner for loop 
            __m128 vf32_x_mul_x = _mm_mul_ps(vf32_x, vf32_x); // vectorised (x * x) 
            __m128 vf32_y_mul_y = _mm_mul_ps(vf32_y, vf32_y); // vectorised (y * y) 
            vf32_x = _mm_sub_ps(vf32_x_mul_x, vf32_y_mul_y); // vectorised x = (x * x) - (y * y) 
            __m128 vf32_y_mul_const_2 = _mm_mul_ps(vf32_y, vf32_const_2); // vectorised y = (y * 2) 
            vf32_y = _mm_mul_ps(vf32_x, vf32_y_mul_const_2); // vectorised y = (x * y * 2) 
            __m128 vf32_cmpgt_x = _mm_cmpgt_ps(vf32_x, vf32_const_1000); // vectorised if (x > 1000)
            __m128 vf32_cmpgt_y = _mm_cmpgt_ps(vf32_y, vf32_const_1000); // vectorised if (y > 1000)

            __m128 vf32_and_x = _mm_and_ps(vf32_cmpgt_x, vf32_const_1);
            __m128 vf32_and_y = _mm_and_ps(vf32_cmpgt_y, vf32_const_1);
            __m128 vf32_or = _mm_or_ps(vf32_and_x, vf32_and_y);
            if (_mm_movemask_ps(vf32_or)) { 
                _mm_storeu_ps(&a[i], vf32_or); // set vectored 4 values of a[i] to 0
                break;
            }
        }
    } 
}
