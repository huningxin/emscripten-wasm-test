#include <wasm_simd128.h>
#include <stdio.h>

__attribute__((noinline))
inline __i32x4 v_dotprod(const __i16x8& a, const __i16x8& b)
{
    v128_t a0 = wasm_i32x4_shr(wasm_i32x4_shl(a, 16), 16);
    v128_t a1 = wasm_i32x4_shr(a, 16);
    v128_t b0 = wasm_i32x4_shr(wasm_i32x4_shl(b, 16), 16);
    v128_t b1 = wasm_i32x4_shr(b, 16);
    v128_t c = wasm_i32x4_mul(a0, b0);
    v128_t d = wasm_i32x4_mul(a1, b1);
    return wasm_i32x4_add(c, d);
}

int main() {
    __i16x8 a = wasm_i16x8_make(0, 1, 2, 3, 4, 5, 6, 7);
    __i16x8 b = wasm_i16x8_make(7, 6, 5, 4, 3, 2, 1, 0);
    __i32x4 c = v_dotprod(a, b);
    printf("%d \n", wasm_i32x4_extract_lane(c, 0));

    return 0;
}