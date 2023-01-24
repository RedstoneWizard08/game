#!/bin/bash

DIR="$(dirname "$0")"

[[ ! -f "configure" ]] && "$DIR/generate-configure.sh" > /dev/null
[[ ! -f "Makefile" ]] && ./configure > /dev/null

make > /dev/null

src/game
