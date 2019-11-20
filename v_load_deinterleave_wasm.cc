#include <wasm_simd128.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>  

inline v128_t wasm_unpacklo_i8x16(v128_t a, v128_t b) {
    // expect punpcklbw
    return wasm_v8x16_shuffle(a, b, 0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23);
}

inline v128_t wasm_unpackhi_i64x2(v128_t a, v128_t b) {
    // expect punpckhqdq
    return wasm_v8x16_shuffle(a, b, 8,9,10,11,12,13,14,15,24,25,26,27,28,29,30,31);
}

__attribute__((noinline))
inline void v_load_deinterleave(const unsigned char* ptr, __u8x16& a, __u8x16& b, __u8x16& c)
{
    v128_t t00 = wasm_v128_load(ptr);
    v128_t t01 = wasm_v128_load(ptr + 16);
    v128_t t02 = wasm_v128_load(ptr + 32);

    v128_t t10 = wasm_unpacklo_i8x16(t00, wasm_unpackhi_i64x2(t01, t01));
    v128_t t11 = wasm_unpacklo_i8x16(wasm_unpackhi_i64x2(t00, t00), t02);
    v128_t t12 = wasm_unpacklo_i8x16(t01, wasm_unpackhi_i64x2(t02, t02));

    v128_t t20 = wasm_unpacklo_i8x16(t10, wasm_unpackhi_i64x2(t11, t11));
    v128_t t21 = wasm_unpacklo_i8x16(wasm_unpackhi_i64x2(t10, t10), t12);
    v128_t t22 = wasm_unpacklo_i8x16(t11, wasm_unpackhi_i64x2(t12, t12));

    v128_t t30 = wasm_unpacklo_i8x16(t20, wasm_unpackhi_i64x2(t21, t21));
    v128_t t31 = wasm_unpacklo_i8x16(wasm_unpackhi_i64x2(t20, t20), t22);
    v128_t t32 = wasm_unpacklo_i8x16(t21, wasm_unpackhi_i64x2(t22, t22));

    a = wasm_unpacklo_i8x16(t30, wasm_unpackhi_i64x2(t31, t31));
    b = wasm_unpacklo_i8x16(wasm_unpackhi_i64x2(t30, t30), t32);
    c = wasm_unpacklo_i8x16(t31, wasm_unpackhi_i64x2(t32, t32));
}

int main() {
    const size_t size = 48 * 1000;
    unsigned char data[size];
    srand (time(NULL));
    for (int i = 0; i < size; ++i)
      data[i] = rand() % 255;;
    __u8x16 a, b, c;
    srand (time(NULL));
    clock_t begin = clock();
    for (int i = 0; i < 100000000; ++i)
      v_load_deinterleave(data + rand() % 100, a, b, c);
    clock_t end = clock();
    printf("result: %d %d %d \n",
           wasm_i8x16_extract_lane(a, 0),
           wasm_i8x16_extract_lane(b, 1),
           wasm_i8x16_extract_lane(c, 2));
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f s\n", time_spent);
    return 0;
}