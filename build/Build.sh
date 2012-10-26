#!/bin/sh
all="all"
if [ $1 = $all ]; then
    echo "Building in all build type (Debug && Release)"
    # Build project
    cmake . -DCMAKE_BUILD_TYPE=Debug
    make
    
    cmake . -DCMAKE_BUILD_TYPE=Release
    make
else
    echo "Building in $1 mode."
    # Build projet in build type of argument
    cmake . -DCMAKE_BUILD_TYPE=$1
    make
fi

# Copy data
sh CopyClientData.sh
sh CopyServerData.sh
