#!/bin/sh

# Build server
cd Server
cmake . -DCMAKE_BUILD_TYPE=$1
make
cd ../bin/Server/$1
gdb ./Server
