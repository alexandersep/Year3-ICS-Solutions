#include <math.h>
#include <immintrin.h>

/* mean is probably meant to be float mean, not float * */
float stddev(float mean, float * samples, int size) {
    float sum = 0, result;
    for (int i = 0; i < size; i++) {
        float temp = samples[i] - mean;
        sum = sum + temp * temp;
    }
    result = sum / (float) size;
    return sqrt(result);
}

// Source: https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction
// Equivalent to 2 hadd's with a 32 bit lower extraction
static inline float hsum_ps_sse3(__m128 v4a) {
    __m128 shuf = _mm_movehdup_ps(v4a);
    __m128 sums = _mm_add_ps(v4a, shuf);
    shuf        = _mm_movehl_ps(shuf, sums);
    sums        = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

float vec_stddev(float mean, float * samples, int size) {
    __m128 vf32_sum = _mm_setzero_ps();
    __m128 vf32_mean = _mm_set1_ps(mean);
    int i;
    int size_minus_three = size - 3;
    for (i = 0; i < size_minus_three; i+=4) {
        __m128 vf32_samples = _mm_loadu_ps(&samples[i]);
        __m128 vf32_temp = _mm_sub_ps(vf32_samples, vf32_mean);
        vf32_temp = _mm_mul_ps(vf32_temp, vf32_temp);
        vf32_sum = _mm_add_ps(vf32_sum, vf32_temp);
    }
    float sum = hsum_ps_sse3(vf32_sum); 
    for (/* i = i */; i < size; i++) {
        float temp = samples[i] - mean;
        sum = sum + temp * temp;
    }
    float result = sum / (float) size;
    return sqrt(result);
}
