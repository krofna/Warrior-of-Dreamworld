#!/bin/sh

# Build Client
cmake . -DCMAKE_BUILD_TYPE=$1 -DBUILD_ONLY_CLIENT=TRUE
make

# Copy Client Data
sh CopyClientData.sh

# Launch client with debugger GDB
cd ../bin/Client/$1/
gdb ./Client
