#include <immintrin.h>

void multiply(float ** restrict matrix, float * restrict vec, float * restrict result) {
    for (int i = 0; i < 4096; i++) {
        float sum = 0.0;
        for (int j = 0; j < 4096; j++) {
            sum += vec[j] * matrix[i][j];
        }
        result[i] = sum;
    }
}

// Source: https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction
// Fast horiztonal sum, equivalent of 2 hadd's and a lower 32 bit extraction
static inline float hsum_ps_sse3(__m128 v4a) {
    __m128 shuf = _mm_movehdup_ps(v4a);
    __m128 sums = _mm_add_ps(v4a, shuf);
    shuf        = _mm_movehl_ps(shuf, sums);
    sums        = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

void vec_multiply(float ** restrict matrix, float * restrict vec, float * restrict result) {
    for (int i = 0; i < 4096; i++) {
        __m128 vf32_sum = _mm_setzero_ps();  
        for (int j = 0; j < 4096; j+=4) { 
            __m128 vf32_vecj = _mm_loadu_ps(&(vec[j]));
            __m128 vf32_matrixij = _mm_loadu_ps(&(matrix[i][j]));
            __m128 vf32_mul = _mm_mul_ps(vf32_vecj, vf32_matrixij);
            vf32_sum = _mm_add_ps(vf32_sum, vf32_mul);
        }
        result[i] = hsum_ps_sse3(vf32_sum); // efficient horiztonal sum vector
    }
}
