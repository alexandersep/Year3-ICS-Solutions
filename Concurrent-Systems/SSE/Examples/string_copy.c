void string_copy(char * restrict src, char * restrict dest, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

/* only load and store don't need to */
void string_copy(char * restrict src, char * restrict dest, int size) {
    int i;
    for (i = 0; i < size - 15; i+=16) {
        __m128 v16source = _mm_loadu_ps((float *)&src[i]);
        _mm_storeu_ps((float *)&dest[i], v16source);
    }
    for (/* i = i */; i < size; i++) {
        dest[i] = src[i];
    }
}

void first_2048_powers(float num, float * result) {
    float power = num;
    for (int i = 0; i < 2048; i++) {
        result[i] = power;
        power *= num;
    }
}

void first_2048_powers(float num, float * result) {
    float power = num;
    float num_2 = num * num;
    float num_3 = num_2 * num;
    float num_4 = num_3 * num;
    __m128 power4 = _mm_setr_ps(num, num_2, num_3, num_4);
    __m128 mul4 = _mm_set1_ps(num_4);
    for (int i = 0; i < 2048; i+=4) {
        _mm_storeu_ps(resuli, power4);
        power4 = _mm_mulps(power4, mul4);
    }
}

const int size = 1048;
float a[size]; /* byte 16 byte aligned */
for (i = 0; i < size-1; i++) {
    a[i] = a[i] + a[i+1];
}

for (i = 0; i < size-4; i+=4) {
    __m128 vf32_a = _mm_load_ps(&a[i]);
    __m128 vf32_aplus1 = _mm_load_ps(&a[i+1]);
    __m128 sum4 = _mm_add_ps(vf32_a,vf32_aplus1);
    _mm_store_ps(&(a[i]), sum4);
}
for (/* i = i */; i < size - 1; i++) {
    a[i] = a[i] + a[i+1];
}

const int size = 1048;
float a[size]; /* byte 16 byte aligned */
for (i = 1; i < size-1; i++) {
    a[i] = a[i-1];
}

float stddev(float mean, float * smamples, int size) {
    float sum = 0, result;
    for (int i = 0; i < size; i++) {
        float temp = samples[i] - mean;
        sum = sum + temp * temp;
    }
    result = sum / (float) size;
    return sqrt(result);
}

static inline float hsum_ps_sse3(__m128 v4a) {
    __m128 shuf = _mm_movehdup_ps(v4a);
    __m128 sums = _mm_add_ps(v4a, shuf);
    shuf        = _mm_movehl_ps(shuf, sums);
    sums        = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

float stddev(float mean, float * smamples, int size) {
    __m128 sum4 = _mm_setzero_ps();
    __m128 mean4 = _mm_set1_ps(mean);
    for (int i = 0; i < size - 3; i+=4) {
        __m128 samples4 = _mm_loadu_ps(&samples[i]);
        __m128 temp4 = _mm_sub_ps(samples4, mean4);
        temp4 = _mm_mul_ps(temp4, temp4);
        sum4 = _mm_add_ps(sum4, temp4);
    }
    float sum = hsum_ps_sse3(sum4); 
    float result;
    for (/* i = i */; i < size; i++) {
        float temp = samples[i] - mean;
        sum = sum + temp * temp;
    }
    result = sum / (float) size;
    return sqrt(result);
}
