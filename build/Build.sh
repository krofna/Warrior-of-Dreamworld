#!/bin/sh

# Build Client
cd Client
cmake . -DCMAKE_BUILD_TYPE=Debug
make
cmake . -DCMAKE_BUILD_TYPE=Release
make
cd ..

# Copy Client's data in Client bin folder
cp -R ../client/Release/* bin/Client/Debug/
cp -R ../client/Release/* bin/Client/Release/

# Build Server
cd Server
cmake . -DCMAKE_BUILD_TYPE=Debug
make
cmake . -DCMAKE_BUILD_TYPE=Release
make
cd ..

# Build World Editor
cd WorldEdit
cmake . -DCMAKE_BUILD_TYPE=Debug
make
cmake . -DCMAKE_BUILD_TYPE=Release
make
cd ..