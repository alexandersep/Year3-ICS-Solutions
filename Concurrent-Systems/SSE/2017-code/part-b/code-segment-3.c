#include <immintrin.h>

float rgb_sum_product(float * pixels) {
    float sum_red = 0;
    float sum_green = 0;
    float sum_blue = 0;
    for (int i = 0; i < 1011; i+=3) {
        sum_red = sum_red + pixels[i];
        sum_green = sum_green + pixels[i+1];
        sum_blue = sum_blue + pixels[i+2];
    }
    return sum_red * sum_green * sum_blue;
}

float vec_rgb_sum_product(float * pixels) {
    __m128 vf32_sum_rgbr = _mm_setzero_ps();
    __m128 vf32_sum_gbrg = _mm_setzero_ps();
    __m128 vf32_sum_brgb = _mm_setzero_ps();
    int i;
    /* Loop and vectorise 4 at a time */
    for (i = 0; i < 1000; i+=12) { /* 1011 - 11 */
        __m128 vf32_pixels_i = _mm_loadu_ps(&pixels[i]);
        __m128 vf32_pixels_i_plus_four = _mm_loadu_ps(&pixels[i+4]);
        __m128 vf32_pixels_i_plus_eight = _mm_loadu_ps(&pixels[i+8]);
        vf32_sum_rgbr = _mm_add_ps(vf32_sum_rgbr, vf32_pixels_i);  
        vf32_sum_gbrg = _mm_add_ps(vf32_sum_gbrg, vf32_pixels_i_plus_four); 
        vf32_sum_brgb = _mm_add_ps(vf32_sum_brgb, vf32_pixels_i_plus_eight); 
    }
    /* shuffle middle vector from [g, b, r, g] to [r, g, b, g] */
    /*                             0  1  2  3      2  0  1  3  */
    vf32_sum_gbrg = _mm_shuffle_ps(vf32_sum_gbrg, vf32_sum_gbrg, _MM_SHUFFLE(3, 1, 0, 2));
    /* shuffle last vector from [b, r, g, b] to [r, g, b, b] */ 
    /*                           0  1  2  3      1  2  3  0  */
    vf32_sum_brgb = _mm_shuffle_ps(vf32_sum_brgb , vf32_sum_brgb, _MM_SHUFFLE(0, 3, 2, 1));

    /* Add 3 vectors that are r g b r | g b r g | b r g b and answer is in [sumr, sumg sumb, invalid] vf32_sumrgb */
    __m128 vf32_sumrgb = _mm_add_ps(vf32_sum_rgbr, vf32_sum_gbrg); 
    vf32_sumrgb = _mm_add_ps(vf32_sumrgb, vf32_sum_brgb); 

    /* Extract values higher lane r,g,b using SSE intrinsics */
    float r = _mm_cvtss_f32(_mm_shuffle_ps(vf32_sum_rgbr, vf32_sum_rgbr, _MM_SHUFFLE(0, 0, 0, 3)));
    float g = _mm_cvtss_f32(_mm_shuffle_ps(vf32_sum_gbrg, vf32_sum_gbrg, _MM_SHUFFLE(0, 0, 0, 3)));
    float b = _mm_cvtss_f32(_mm_shuffle_ps(vf32_sum_brgb, vf32_sum_brgb, _MM_SHUFFLE(0, 0, 0, 3)));

    /* Loop and vectorise 3 at a time only */
    for (/* i = i */; i < 1011; i+=3) { 
        __m128 vf32_pixels = _mm_loadu_ps(&pixels[i]);
        vf32_sumrgb = _mm_add_ps(vf32_sumrgb, vf32_pixels);
    }
    /* Extract using SSE intrinsics [r, g, b, invalid] */
    r += _mm_cvtss_f32(vf32_sumrgb);
    g += _mm_cvtss_f32(_mm_shuffle_ps(vf32_sumrgb, vf32_sumrgb, _MM_SHUFFLE(0, 3, 2, 1)));
    b += _mm_cvtss_f32(_mm_shuffle_ps(vf32_sumrgb, vf32_sumrgb, _MM_SHUFFLE(0, 0, 3, 2)));
    return r * g * b;
}
