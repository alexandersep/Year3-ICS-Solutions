#include <stdbool.h>
#include <immintrin.h>

// I'm gonna assume string[2] was a mistake and it meant string[i]
bool strcmp_4(char* string1, char* string2, int strlength) {
    for (int i = 0; i < strlength; i++) {
        if (string1[i] != string2[i]) {
            return false;
        }
    }
    return true;
}

// I'm gonna assume string[2] was a mistake and it meant string[i]
bool vec_strcmp_4(char* string1, char* string2, int strlength) {
    int i;
    int strlength_minus_fifteen = strlength - 15;
    for (i = 0; i < strlength_minus_fifteen; i+=16) { // 16 characters make 128 bits
        __m128 vf32_string1 = _mm_loadu_ps((float*)&string1[i]); // 4 lanes of 16 characters
        __m128 vf32_string2 = _mm_loadu_ps((float*)&string2[i]); // 4 lanes of 16 characters
        __m128 vf32_cmp = _mm_cmpeq_ps(vf32_string1, vf32_string2);
        int mask = _mm_movemask_ps(vf32_cmp);
        if (mask != 0xf) {
            return false;
        }
    }
    for (i = 0; i < strlength; i++) {
        if (string1[i] != string2[i]) {
            return false;
        }
    }
    return true;
}

#if 0
/* Inefficient implementation using slow string intrinsics */
bool vec_strcmp_4(char* string1, char* string2, int strlength) {
    int i;
    for(i = 0; i < strlength - 15; i+=16) {
        __m128i v16string1 = _mm_loadu_si128((__m128i*)&(string1[i]));
        __m128i v16string2 = _mm_loadu_si128((__m128i*)&(string2[i]));
        if (_mm_cmpistrc(v16string1,v16string2,_SIDD_UBYTE_OPS |
                                               _SIDD_CMP_EQUAL_EACH |
                                               _SIDD_NEGATIVE_POLARITY |
                                               _SIDD_LEAST_SIGNIFICANT)) {
            int index = _mm_cmpistri(v16string1,v16string2, _SIDD_UBYTE_OPS |
                                                           _SIDD_CMP_EQUAL_EACH |
                                                           _SIDD_NEGATIVE_POLARITY |
                                                           _SIDD_LEAST_SIGNIFICANT);
            if (string1[index] != string2[index])
                return false;
        } else if (_mm_cmpistrz(v16string1,v16string2,_SIDD_UBYTE_OPS |
                                                      _SIDD_CMP_EQUAL_EACH |
                                                      _SIDD_NEGATIVE_POLARITY |
                                                      _SIDD_LEAST_SIGNIFICANT)) {
            return false;
        }
    }
    // postloop
    for (/* i = i */; i < strlength; i++) {
        if (string1[i] != string2[i])
            return false;
    }
    return true;
}
#endif
