WASM_SRC_FILES := $(wildcard *_wasm.cc)
HTML_FILES := $(patsubst %.cc,%.html, $(WASM_SRC_FILES))
WAT_FILES := $(patsubst %.cc,%.wat, $(WASM_SRC_FILES))
WASM_FILES := $(patsubst %.cc,%.wasm, $(WASM_SRC_FILES))
JS_FILES := $(patsubst %.cc,%.js, $(WASM_SRC_FILES))
SSE_SRC_FILES := $(wildcard *_sse.cc)
SSE_EXES := $(patsubst %.cc,%, $(SSE_SRC_FILES))
WASM_CPPFLAGS := -O2 -g2 -msimd128
SSE_CPPFLAGS := -O2 -g2 -msse3 -flax-vector-conversions

.PHONY: all
all: $(HTML_FILES) $(WAT_FILES) $(SSE_EXES)

%_sse: %_sse.cc
	clang $(SSE_CPPFLAGS) -o $@ $<

%_wasm.html: %_wasm.cc
	emcc $(WASM_CPPFLAGS) -o $@ $<

%.wat: %.html
	wasm-dis -o $@ $(patsubst %.html,%.wasm,$<)

clean:
	rm -f $(HTML_FILES) $(WAT_FILES) $(SSE_EXES) $(WASM_FILES) $(JS_FILES)
