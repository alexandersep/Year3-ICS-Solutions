//
// CSU33014 Annual Exam, May 2022
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
// [2, 3, 4, 5, 6]
// product = 1, [2..]
// product = 2, [2, 6..]
// product = 6, [2,6,24..]
// product = 20, [2,6,24,120,720] ...
// in the following, size can have any positive value
float Q1_routine_1(float * restrict a, float * restrict b, int size) {
    float product_a = 1.0;
    float product_b = 1.0;

    for (int i = 0; i < size; i++) {
        // fprintf(stderr, "pa %f, ai %f, pb %f, bi %f\n", product_a, a[i], product_b, b[i]);
        product_a = product_a * a[i];
        product_b = product_b * b[i];
    }
    return product_a + product_b;
}

// Source: https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction
// Fast horizonal multiplication using SSE3
// Similar to two hadds's but with multiplying and a return lowest lane for float vectors
static inline float hmul_ps_sse3(__m128 v4a) { // v4a = [1, 2, 3, 4]
    __m128 shuf = _mm_movehdup_ps(v4a); // shuf = [2, 2, 4, 4]
    __m128 muls = _mm_mul_ps(v4a, shuf); // muls = v4a * shuf = [2, 4, 12, 16]
    shuf        = _mm_movehl_ps(shuf, muls); // shuf = [12, 16, 4, 4]
    muls        = _mm_mul_ss(muls, shuf); // muls * shuf = [24, ?, ?, ?]
    return _mm_cvtss_f32(muls); // return 24 == 1 * 2 * 3 * 4 = 24
}

// in the following, size can have any positive value
float Q1_vectorized_1(float * restrict a, float * restrict b, int size) {
    float product_a = 1.0; // if size >= 4 then postloop else useless assignment
    float product_b = 1.0; 
    int i;  
    if (size >= 4) { // saves the need of using _mm_set1_ps(a[0]) twice if condition is bad
        __m128 v4product_a = _mm_loadu_ps(&a[0]); 
        __m128 v4product_b = _mm_loadu_ps(&b[0]); 
        int size_minus_three = size - 3;
        for (i = 4; i < size_minus_three; i+=4) {
            __m128 v4a = _mm_loadu_ps(&a[i]); 
            __m128 v4b = _mm_loadu_ps(&b[i]); 
            v4product_a = _mm_mul_ps(v4product_a, v4a);
            v4product_b = _mm_mul_ps(v4product_b, v4b);
        }
        product_a = hmul_ps_sse3(v4product_a);
        product_b = hmul_ps_sse3(v4product_b); 
    }
    for (/* i = i */; i < size; i++) { // else or end or postloop
        product_a = product_a * a[i];
        product_b = product_b * b[i];
    }
    return product_a + product_b; 
}

/******************* routine 2 ***********************/

// in the following, size can have any positive value
void Q1_routine_2(float * restrict a, float * restrict b, int size) {
    for ( int i = 0; i < size; i++ ) {
        a[i] = (a[i] * 17.2) + (3.14159/b[i]);
    }
}

// in the following, size can have any positive value
void Q1_vectorized_2(float * restrict a, float * restrict b, int size) {
    int i;
    int size_minus_three = size - 3; // optimized out computation for for loop
    __m128 vf32_pi = _mm_set1_ps(3.14159);
    __m128 vf32_scalar = _mm_set1_ps(17.2);
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_a = _mm_loadu_ps(&a[i]);
        __m128 vf32_b = _mm_loadu_ps(&b[i]);
        __m128 vf32_mul = _mm_mul_ps(vf32_a, vf32_scalar);
        __m128 vf32_div = _mm_div_ps(vf32_pi, vf32_b);
        vf32_a = _mm_add_ps(vf32_mul, vf32_div);
        _mm_storeu_ps(&a[i], vf32_a); 
    }
    for (/* i = i */; i < size; i++) {
        a[i] = (a[i] * 17.2) + (3.14159/b[i]);
    }
}

/******************** routine 3 ************************/

// in the following, size can have any positive value
void Q1_routine_3(float * restrict a, float * restrict b, int size) {
    for ( int i = 0; i < size; i++ ) {
        if ( a[i] > b[i] ) {
            a[i] = -a[i];
        }
    }
}

// in the following, size can have any positive value
void Q1_vectorized_3(float * restrict a, float * restrict b, int size) {
    int i;
    int size_minus_three = size - 3; // optimized out computation for for loop
    __m128 vf32_zero = _mm_setzero_ps();
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_a = _mm_loadu_ps(&a[i]); // [1, 2, 3, 4]
        __m128 vf32_b = _mm_loadu_ps(&b[i]); // [-1, 0, 4, 5]
        __m128 vf32_cmpgt = _mm_cmpgt_ps(vf32_a, vf32_b); // [0xfffff, 0xfffff, 0, 0]
        __m128 vf32_and_a_cmpgt = _mm_and_ps(vf32_a, vf32_cmpgt); // [1, 2, 0, 0]
        __m128 vf32_sub_a = _mm_sub_ps(vf32_zero, vf32_and_a_cmpgt); // [-1, -2, 0, 0]
        __m128 vf32_and_a_cmple = _mm_andnot_ps(vf32_cmpgt, vf32_a); // [0, 0, 0xffff, 0xffff] -> [0, 0, 3, 4]
        __m128 vf32_negated = _mm_or_ps(vf32_and_a_cmple, vf32_sub_a); // [-1, -2, 3, 4] 
        _mm_storeu_ps(&a[i], vf32_negated); // a[i] == 4
    }
    for (/* i = i */; i < size; i++ ) {
        if (a[i] > b[i]) {
            a[i] = -a[i];
        }
    }
}

/********************* routine 4 ***********************/

void Q1_routine_4(float * restrict a, float * restrict b, float * restrict c) {
    for ( int i = 0; i < 2048; i++  ) {
        if ( i == 0 ) {
            a[i] = b[i]*c[i];
        }
        else if ( i == 2047 ) {
            a[i] = b[i]*c[i];
        }
        else {
            a[i] = b[i-1]*c[i+1] + b[i]*c[i] + b[i+1]*c[i-1];
        }
    }
}

/* Correct but slow 1 at a time approach using vectors
// Fast horizonal add using SSE3
// Equivalent to two hadds's 
static inline float hsum_ps_sse3(__m128 v4a) {
    __m128 shuf = _mm_movehdup_ps(v4a);
    __m128 muls = _mm_add_ps(v4a, shuf);
    shuf        = _mm_movehl_ps(shuf, muls);
    muls        = _mm_add_ss(muls, shuf);
    return _mm_cvtss_f32(muls);
}

void Q1_vectorized_4(float * restrict a, float * restrict b, float * restrict c) {
    a[0] = b[0] * c[0];
    int limit = 2046;
    __m128i vi32_mask = _mm_set_epi32(0, 0xffffffff, 0xffffffff, 0xffffffff);
    __m128 vf32_mask = _mm_castsi128_ps(vi32_mask);
    for (int i = 0; i < limit; i++) { // computes 2047 as well, but will need recomputing (faster this way)
        __m128 vf32_b = _mm_loadu_ps(&b[i]); 
        __m128 vf32_c = _mm_loadu_ps(&c[i]); 
        vf32_c = _mm_shuffle_ps(vf32_c, vf32_c, _MM_SHUFFLE(3, 0, 1, 2)); // reorder so vertical mul 
        __m128 vf32_mul = _mm_mul_ps(vf32_b, vf32_c);
        __m128 vf32_masked = _mm_and_ps(vf32_mul, vf32_mask); // extract only first 3 lanes
        a[i+1] = hsum_ps_sse3(vf32_masked);
    }
    a[2047] = b[2047]*c[2047]; // recompute the correct value 
}
*/

// t0x helped with getting me a solution with 6 loads and looping 4 at a time
// it was then improved by me (Alexander) with the loop unrolling if statement
// and cache prediction with moving a[2047] at the end.
// It was then later improved by The spectacular Robert Gaynor, IG handle: rogob101 regarding
// optimising two loads and using shuffles
void Q1_vectorized_4(float * restrict a, float * restrict b, float * restrict c) {
    a[0] = b[0] * c[0];
    int i;
    for (i = 1; i < 2045; i+=4) {
        /* Load i-1, i, i+1 for 3 separate vectors */
        __m128 vf32_b_minus = _mm_loadu_ps(&b[i-1]);        
        __m128 vf32_b_plus = _mm_loadu_ps(&b[i+1]);        
        __m128 vf32_b = _mm_shuffle_ps(vf32_b_minus, vf32_b_plus, _MM_SHUFFLE(2, 1, 2, 1)); // optimise load instruction by reusing b[i-1] b[i+1] values
        __m128 vf32_c_minus = _mm_loadu_ps(&c[i-1]);        
        __m128 vf32_c_plus = _mm_loadu_ps(&c[i+1]);        
        __m128 vf32_c = _mm_shuffle_ps(vf32_c_minus, vf32_c_plus, _MM_SHUFFLE(2, 1, 2, 1)); // optimise load instruction by reusing c[i-1] c[i+1] values
        
        __m128 vf32_mul_minus = _mm_mul_ps(vf32_b_minus,vf32_c_plus);
        __m128 vf32_mul = _mm_mul_ps(vf32_b, vf32_c);
        __m128 vf32_mul_plus = _mm_mul_ps(vf32_b_plus, vf32_c_minus);
        __m128 vf32_answer = _mm_add_ps(_mm_add_ps(vf32_mul_minus, vf32_mul), vf32_mul_plus);
        _mm_storeu_ps(&a[i],vf32_answer);
    }
    if (i < 2047) {  // loop unroll the remaining 2 guaranteed elements if (i < 2047)
        a[2045] = b[2044]*c[2046] + b[2045]*c[2045] + b[2046]*c[2044];
        a[2046] = b[2045]*c[2047] + b[2046]*c[2046] + b[2047]*c[2045];
    }
    a[2047] = b[2047] * c[2047]; // at the end and not start for cache better cache hit
}

/********************* routine 5 ***********************/

// in the following, size can have any positive value
void Q1_routine_5(float * restrict a, float * restrict b, float * restrict c, int size) {
    for ( int i = 0; i < size; i++ ) {
        if ( a[i] > c[i] ) { // if a > c
            float temp = a[i]; // swap values of a with c into a and c respectively
            a[i] = c[i];
            c[i] = temp;
        }
        if ( a[i] > b[i] ) {
            float temp = a[i];
            a[i] = b[i];
            b[i] = temp; 
        }
        if ( b[i] > c[i] ) {
            float temp = b[i];
            b[i] = c[i];
            c[i] = temp;
        }
    }
}

// in the following, size can have any positive value
void Q1_vectorized_5(float * restrict a, float * restrict b, float * restrict c, int size) {
    int i;
    int size_minus_three = size - 3; // optimized computation for for loop
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_a = _mm_loadu_ps(&a[i]);
        __m128 vf32_b = _mm_loadu_ps(&b[i]);
        __m128 vf32_c = _mm_loadu_ps(&c[i]);

        // if (a[i] > c[i]) vectors swap 
        __m128 vf32_tempa = _mm_min_ps(vf32_a, vf32_c);
        vf32_c = _mm_max_ps(vf32_a, vf32_c);

        // if (a[i] > b[i]) vectors swap 
        vf32_a = _mm_min_ps(vf32_tempa, vf32_b);
        vf32_b = _mm_max_ps(vf32_tempa, vf32_b);
        
        // if (b[i] > c[i]) vectors swap 
        __m128 vf32_tempb = _mm_min_ps(vf32_b, vf32_c);
        vf32_c = _mm_max_ps(vf32_b, vf32_c);

        _mm_storeu_ps(&a[i], vf32_a);
        _mm_storeu_ps(&b[i], vf32_tempb);
        _mm_storeu_ps(&c[i], vf32_c);
    }
    // i = i 
    for (; i < size; i++) {
        if (a[i] > c[i]) {
            float temp = a[i];
            a[i] = c[i];
            c[i] = temp;
        }
        if (a[i] > b[i]) {
            float temp = a[i];
            a[i] = b[i];
            b[i] = temp; 
        }
        if (b[i] > c[i]) {
            float temp = b[i];
            b[i] = c[i];
            c[i] = temp;
        }
    }
}

#if 0
/* Slower by 2 latency each if statement from original code, but does not use max and min */
void Q1_vectorized_5(float * restrict a, float * restrict b, float * restrict c, int size) {
    int i;
    for (i = 0; i < (size-3); i=i+4 ) {
        __m128 vf32_a = _mm_loadu_ps(&a[i]);
        __m128 vf32_b = _mm_loadu_ps(&b[i]);
        __m128 vf32_c = _mm_loadu_ps(&c[i]);

        // a = [1,2,3,4]; c = [2,1,2,3]
        //( a[i] > c[i] )
        __m128 vf32_cmpgt = _mm_cmpgt_ps(vf32_a, vf32_c);  // [0,0xf,0xf,0xf] 
       
        // vf32_or_a_c max values from "a" and "c"
        __m128 vf32_cmpgt_first = _mm_and_ps(vf32_cmpgt, vf32_a); // [0,2,3,4] 
        __m128 vf32_cmple_second = _mm_andnot_ps(vf32_cmpgt, vf32_c); // [2,0,0,0]
        __m128 vf32_max = _mm_or_ps(vf32_cmpgt_first,vf32_cmple_second); // [2,2,3,4]

        vf32_cmpgt_first = _mm_and_ps(vf32_cmpgt, vf32_c); // [0,1,2,3] 
        vf32_cmple_second = _mm_andnot_ps(vf32_cmpgt, vf32_a); // [1,0,0,0]
        __m128 vf32_min = _mm_or_ps(vf32_cmpgt_first,vf32_cmple_second); // [1,1,2,3]
        
        // a = [1,1,2,3]; b = [2,1,2,3]
        //( a[i] > b[i] )
        vf32_cmpgt = _mm_cmpgt_ps(vf32_min, vf32_b);  // [0,0,0,0]
        
        // vf32_or_a_c max values from "a" and "c"
        vf32_cmpgt_first = _mm_and_ps(vf32_cmpgt, vf32_min); // [0,0,0,0]
        vf32_cmple_second = _mm_andnot_ps(vf32_cmpgt, vf32_b); // [2,1,2,3]
        __m128 vf32_max_b = _mm_or_ps(vf32_cmpgt_first,vf32_cmple_second); // [2,1,2,3]
       
        vf32_cmpgt_first = _mm_and_ps(vf32_cmpgt, vf32_b); // [0,1,2,3] 
        vf32_cmple_second = _mm_andnot_ps(vf32_cmpgt, vf32_min); // [1,0,0,0]
        _mm_storeu_ps(&a[i], _mm_or_ps(vf32_cmpgt_first,vf32_cmple_second)); // [1,1,2,3]
        
        // b = [2,1,2,3] c = [2,2,3,4]
        //( b[i] > c[i] )
        vf32_cmpgt = _mm_cmpgt_ps(vf32_max_b, vf32_max);  // [0,0,0,0]
        
        // vf32_or_a_c max values from "a" and "c"
        vf32_cmpgt_first = _mm_and_ps(vf32_cmpgt, vf32_max_b); // [0,0,0,0]
        vf32_cmple_second = _mm_andnot_ps(vf32_cmpgt, vf32_max); // [2,2,2,4] 
        _mm_storeu_ps(&c[i], _mm_or_ps(vf32_cmpgt_first,vf32_cmple_second)); // [2,2,2,4]
        
        vf32_cmpgt_first = _mm_and_ps(vf32_cmpgt, vf32_max); // [0,0,0,0]
        vf32_cmple_second = _mm_andnot_ps(vf32_cmpgt, vf32_max_b); // [2,1,2,3] 
        _mm_storeu_ps(&b[i], _mm_or_ps(vf32_cmpgt_first,vf32_cmple_second)); // [2,1,2,3]
    }
    for (i=i; i < size; i++)
    {
        if ( a[i] > c[i] ) {
            float temp = a[i];
            a[i] = c[i];
            c[i] = temp;
        }
        if ( a[i] > b[i] ) {
            float temp = a[i];
            a[i] = b[i];
            b[i] = temp; 
        }
        if ( b[i] > c[i] ) {
            float temp = b[i];
            b[i] = c[i];
            c[i] = temp;
        }
    }
}
#endif

/********************* routine 6 ***********************/

// in the following size is a positive value that is a multiple of 3
float Q1_routine_6(float * restrict a, int size) {
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    for (int i = 0; i < size; i+=3) {
        x = x + a[i];
        y = y + a[i+1];
        z = z + a[i+2];
    }
    return x * y * z;
}

#if 0
/* Shorter but underutilisation implementaton */
float Q1_vectorized_6(float * restrict a, int size) {
    int i;
    __m128 vf32_acc = _mm_setzero_ps(); // accumulator
    for (i = 0; i < size; i+=3) { // unroll loop if you want (fix if statement to loop though
        __m128 vf32_a = _mm_loadu_ps(&a[i]);
        vf32_acc = _mm_add_ps(vf32_acc, vf32_a);
    }
    float x = _mm_cvtss_f32(vf32_acc);
    float y = _mm_cvtss_f32(_mm_shuffle_ps(vf32_acc, vf32_acc, _MM_SHUFFLE(0, 3, 2, 1)));
    float z = _mm_cvtss_f32(_mm_shuffle_ps(vf32_acc, vf32_acc, _MM_SHUFFLE(0, 0, 3, 2)));
    return x * y * z;
}
#endif

/*
sum_xyzx
sum_yzxy
sum_zxyz
x y z x | y z x y | z x y z +
x y z x | y z x y | z x y z +
x y z x | y z x y | z x y z

Answer 
(x y z) (x | y z) (x y | z) (x y z)
x y z x | y z x y | x y z z // shuffle last vector
x y z x | x y z y | x y z z // shuffle middle vector
x y z x | x y z y | x y z z // Add 3 vectors 
sumx * sumy * sumz
*/

// in the following size is a positive value that is a multiple of 3
float Q1_vectorized_6(float * restrict a, int size) {
    int i;
    int size_minus_eleven = size - 11; // optimise for "for loop"
    __m128 vf32_sumxyzx = _mm_setzero_ps();
    __m128 vf32_sumyzxy = vf32_sumxyzx; // optimise _mm_setzero_ps() by loading vector already set to zero
    __m128 vf32_sumzxyz = vf32_sumxyzx; // optimise _mm_setzero_ps() by loading vector already set to zero
    for (i = 0; i < size_minus_eleven; i+=12) { 
        __m128 vf32_next_sumxyzx = _mm_loadu_ps(&a[i]);
        __m128 vf32_next_sumyzxy = _mm_loadu_ps(&a[i+4]);
        __m128 vf32_next_sumzxyz = _mm_loadu_ps(&a[i+8]);
        vf32_sumxyzx = _mm_add_ps(vf32_sumxyzx, vf32_next_sumxyzx); 
        vf32_sumyzxy = _mm_add_ps(vf32_sumyzxy, vf32_next_sumyzxy); 
        vf32_sumzxyz = _mm_add_ps(vf32_sumzxyz, vf32_next_sumzxyz); 
    }
    /* shuffle middle vector from [y, z, x, y] to [x, y, z, y] */
    /*                             0  1  2  3      2  0  1  3  */
    vf32_sumyzxy = _mm_shuffle_ps(vf32_sumyzxy, vf32_sumyzxy, _MM_SHUFFLE(3, 1, 0, 2));
    /* shuffle last vector from [z, x, y, z] to [x, y, z, z] */ 
    /*                           0  1  2  3      1  2  3  0  */
    vf32_sumzxyz = _mm_shuffle_ps(vf32_sumzxyz, vf32_sumzxyz, _MM_SHUFFLE(0, 3, 2, 1));

    // [x, y, z, x], [x, y, z, y], [x, y, z, z]
    // [x, y, z, ?]
    /* Add 3 vectors that are x y z x | x y z y | x y z z and answer is in [sumx, sumy sumz, invalid] vf32_sumxyz */
    __m128 vf32_sumxyz = _mm_add_ps(vf32_sumxyzx, vf32_sumyzxy); 
    vf32_sumxyz = _mm_add_ps(vf32_sumxyz, vf32_sumzxyz); 

    /* Extract values higher lane x,y,z using SSE intrinsics */
    float x = _mm_cvtss_f32(_mm_shuffle_ps(vf32_sumxyzx, vf32_sumxyzx, _MM_SHUFFLE(0, 0, 0, 3)));
    float y = _mm_cvtss_f32(_mm_shuffle_ps(vf32_sumyzxy, vf32_sumyzxy, _MM_SHUFFLE(0, 0, 0, 3)));
    float z = _mm_cvtss_f32(_mm_shuffle_ps(vf32_sumzxyz, vf32_sumzxyz, _MM_SHUFFLE(0, 0, 0, 3)));

    for (/* i = i */; i < size; i+=3) {
        __m128 vf32_a = _mm_loadu_ps(&a[i]);
        vf32_sumxyz = _mm_add_ps(vf32_sumxyz, vf32_a);
    }
    /* Extract using SSE intrinsics [x, y, z, invalid] */
    x += _mm_cvtss_f32(vf32_sumxyz);
    y += _mm_cvtss_f32(_mm_shuffle_ps(vf32_sumxyz, vf32_sumxyz, _MM_SHUFFLE(0, 3, 2, 1)));
    z += _mm_cvtss_f32(_mm_shuffle_ps(vf32_sumxyz, vf32_sumxyz, _MM_SHUFFLE(1, 0, 3, 2)));

    return x * y * z;
}
