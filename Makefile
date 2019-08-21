SRC_FILES := $(wildcard *.cc)
HTML_FILES := $(patsubst %.cc,%.html,$(SRC_FILES))
WAT_FILES := $(patsubst %.cc,%.wat,$(SRC_FILES))
CPPFLAGS := -O2 -g2 -msimd128

.PHONY: all
all: $(HTML_FILES) $(WAT_FILES)

%.html: %.cc
	emcc $(CPPFLAGS) -o $@ $<

%.wat: %.html
	wasm2wat --enable-all -o $@ $(patsubst %.html,%.wasm,$<)

clean:
	rm -f *.html; rm -f *.js; rm -f *.wasm; rm -f *.wat