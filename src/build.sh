#!/bin/sh -ex
#~/emscripten/emcc -I ~/gmp.js -g -c -o $1.bc $1.cc
#~/emscripten/emcc $1.bc ~/gmp.js/.libs/libgmp.a -o complete-$1.bc
#llvm-dis -show-annotations complete-$1.bc -o complete-$1.ll
~/emscripten/emcc -g -s INIT_STACK=1 -s ASSERTIONS=1 -s CORRECT_ROUNDINGS=1 -s CORRECT_OVERFLOWS=1 -s SAFE_HEAP=1 -s EXCEPTION_DEBUG=1 -I ~/gmp.js -O0 --closure 0 $1.c* ~/gmp.js/.libs/libgmp.a -o $1-unopt.js
#~/emscripten/emcc -g -I ~/gmp.js -O0 --closure 0 $1.c* ~/gmp.js/.libs/libgmp.a -o $1-unopt.js
#mv -v a.out.js $1-unopt.js
