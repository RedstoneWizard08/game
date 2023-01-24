#!/bin/bash

set -e

FILES=(
    CMakeCache.txt Makefile cmake_install.cmake CMakeFiles build game aclocal.m4 \
    autom4te.cache configure compile install-sh missing Makefile.in config.log \
    config.status confdefs.h configure~ depcomp .deps game.html game.js libtool \
    ltmain.sh .libs .deps .dirstamp game.worker.js config.sub config.guess
)

rm -rf "${FILES[@]}"

cd src

rm -rf "${FILES[@]}"

find . -type f -name "*.o" -exec rm -f {} \;
find . -type f -name "*.a" -exec rm -f {} \;
find . -type f -name "*.lo" -exec rm -f {} \;
find . -type f -name "*.la" -exec rm -f {} \;
find . -type f -name "*.so" -exec rm -f {} \;
find . -type f -name "*.dll" -exec rm -f {} \;
find . -type f -name "*.exe" -exec rm -f {} \;
find . -type f -name "*.wasm" -exec rm -f {} \;
find . -type f -name "*.log" -exec rm -f {} \;
