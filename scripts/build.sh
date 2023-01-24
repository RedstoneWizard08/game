#!/bin/bash

DIR="$(dirname "$0")"

"$DIR/configure.sh"
make "$@"
