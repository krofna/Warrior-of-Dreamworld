#!/bin/sh

# Build Client
cd Client
cmake . -DCMAKE_BUILD_TYPE=$1
make
cd ../bin/Client/$1/
gdb ./Client
