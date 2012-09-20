#!/bin/sh

# Build world edit
cmake . -DCMAKE_BUILD_TYPE=$1 -DBUILD_ONLY_WORLD_EDIT=TRUE
make
