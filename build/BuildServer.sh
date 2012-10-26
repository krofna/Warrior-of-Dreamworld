#!/bin/sh

# For building server
cmake . -DCMAKE_BUILD_TYPE=$1 -DBUILD_SERVER=TRUE
make

# Launch Server
cd ../bin/Server/$1
gdb ./Server
