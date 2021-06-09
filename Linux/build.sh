#!/bin/bash

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

cd $SCRIPT_DIR
mkdir -p build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../ArmLinuxToolChain.txt ..
make
mkdir -p ../install
make DESTDIR=../install install

