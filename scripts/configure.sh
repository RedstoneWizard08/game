#!/bin/bash

set -e

DIR="$(dirname "$0")"

[[ ! -f "configure" ]] && "$DIR/generate-configure.sh"

./configure "$@"
