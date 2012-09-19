#!/bin/sh

# Build project
cmake . -DCMAKE_BUILD_TYPE=Debug
make
cmake . -DCMAKE_BUILD_TYPE=Release
make

# Copy data
sh CopyClientData.sh
sh CopyServerData.sh
