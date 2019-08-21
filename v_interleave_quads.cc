#include <wasm_simd128.h>
#include <stdio.h>

__attribute__((noinline))
inline __i8x16 v_interleave_quads(const __i8x16& vec)
{
    return wasm_v8x16_shuffle(vec, vec, 0,4,1,5,2,6,3,7,8,12,9,13,10,14,11,15);
}

int main() {
    // invoke v_interleave_quads
    __i8x16 a= wasm_i8x16_make(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    __i8x16 b = v_interleave_quads(a);
    printf("%d \n", wasm_i8x16_extract_lane(b, 0));

    return 0;
}