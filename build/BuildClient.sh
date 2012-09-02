#!/bin/sh

# Build Client
cd Client
cmake . -DCMAKE_BUILD_TYPE=$1
make

# Copy Client Data
cp -R ../ClientData/* ../bin/Client/$1/

# Launch client with debugger GDB
cd ../bin/Client/$1/
gdb ./Client
