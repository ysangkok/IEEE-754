#!/bin/sh -ex
EMCC_DEBUG=1 ~/emscripten/emcc -O2 -s EXPORTED_FUNCTIONS="['_btd2','_dtb2']" -s PRECISE_I32_MUL=1 -I ~/gmp.js src/concat.links.c libgmp.a -o concat.links.js
