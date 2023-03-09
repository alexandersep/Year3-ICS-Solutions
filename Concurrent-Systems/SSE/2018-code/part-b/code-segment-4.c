#include <stdio.h>
#include <stdbool.h>
#include <immintrin.h>

bool strcmp_sse(char* string1, char* string2, int strlength) {
    int i;
    for(i = 0; i < strlength - 15; i+=16) {
        __m128i v16string1 = _mm_load_si128((__m128i*)&(string1[i]));
        __m128i v16string2 = _mm_load_si128((__m128i*)&(string2[i]));
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

int main() {
    const int strlength = 19; 
    char* string1 = (char *)_mm_malloc(sizeof(char) * strlength, 16);
    string1[0] = 'S'; string1[1] = 'i'; string1[2] = 'l'; string1[3] = 'e';
    string1[4] = 'n'; string1[5] = 't'; string1[6] = ' '; string1[7] = 'w';
    string1[8] = 'a'; string1[9] = 's'; string1[10] = ' '; string1[11] = 'h';
    string1[12] = 'e'; string1[13] = 'r'; string1[14] = 'e'; string1[15] = ' '; 
    string1[16] = '>'; string1[17] = ':'; string1[18] = ')';
    char* string2 = (char *)_mm_malloc(sizeof(char) * strlength, 16);
    string2[0] = 'S'; string2[1] = 'i'; string2[2] = 'l'; string2[3] = 'e';
    string2[4] = 'n'; string2[5] = 't'; string2[6] = ' '; string2[7] = 'w';
    string2[8] = 'a'; string2[9] = 's'; string2[10] = ' '; string2[11] = 'h';
    string2[12] = 'e'; string2[13] = 'r'; string2[14] = 'e'; string2[15] = ' ';
    string2[16] = '>'; string2[17] = ':'; string2[18] = ')';
    strcmp_sse(string1,string2,strlength) ? printf("Matched\n") : printf("Unmatched\n");
    _mm_free(string1);
    _mm_free(string2);
    return 0;
}
