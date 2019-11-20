#include <emmintrin.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>

// gcc v_load_interleave_sse.cc -flax-vector-conversions -msse3 -O2 -g2

void v_load_deinterleave(const unsigned char * ptr, __v16qu& a, __v16qu& b, __v16qu& c) __attribute__((noinline));

void v_load_deinterleave(const unsigned char * ptr, __v16qu& a, __v16qu& b, __v16qu& c)
{
    __m128i t00 = _mm_loadu_si128((const __m128i*)ptr);
    __m128i t01 = _mm_loadu_si128((const __m128i*)(ptr + 16));
    __m128i t02 = _mm_loadu_si128((const __m128i*)(ptr + 32));

    __m128i t10 = _mm_unpacklo_epi8(t00, _mm_unpackhi_epi64(t01, t01));
    __m128i t11 = _mm_unpacklo_epi8(_mm_unpackhi_epi64(t00, t00), t02);
    __m128i t12 = _mm_unpacklo_epi8(t01, _mm_unpackhi_epi64(t02, t02));

    __m128i t20 = _mm_unpacklo_epi8(t10, _mm_unpackhi_epi64(t11, t11));
    __m128i t21 = _mm_unpacklo_epi8(_mm_unpackhi_epi64(t10, t10), t12);
    __m128i t22 = _mm_unpacklo_epi8(t11, _mm_unpackhi_epi64(t12, t12));

    __m128i t30 = _mm_unpacklo_epi8(t20, _mm_unpackhi_epi64(t21, t21));
    __m128i t31 = _mm_unpacklo_epi8(_mm_unpackhi_epi64(t20, t20), t22);
    __m128i t32 = _mm_unpacklo_epi8(t21, _mm_unpackhi_epi64(t22, t22));

    a = _mm_unpacklo_epi8(t30, _mm_unpackhi_epi64(t31, t31));
    b = _mm_unpacklo_epi8(_mm_unpackhi_epi64(t30, t30), t32);
    c = _mm_unpacklo_epi8(t31, _mm_unpackhi_epi64(t32, t32));
}

int main() {
    const size_t size = 48 * 1000;
    unsigned char data[size];
    srand (time(NULL));
    for (int i = 0; i < size; ++i)
      data[i] = rand() % 255;;
    __v16qu a, b, c;
    srand (time(NULL));
    clock_t begin = clock();
    for (int i = 0; i < 100000000; ++i)
      v_load_deinterleave(data + rand() % 100, a, b, c);
    clock_t end = clock();
    printf("result: %d %d %d \n",
           _mm_cvtsi128_si32(a),
           _mm_cvtsi128_si32(b),
           _mm_cvtsi128_si32(c));
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f s\n", time_spent);
    return 0;
}