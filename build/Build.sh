#!/bin/sh
all="all"
if [ $1 = $all ]; then
    echo "Building in all build type (Debug && Release)"
    # Build project
    cmake . -DCMAKE_BUILD_TYPE=Debug -DBUILD_SERVER=TRUE -DBUILD_CLIENT=TRUE -DBUILD_WORLD_EDIT=TRUE
    make
    
    cmake . -DCMAKE_BUILD_TYPE=Release -DBUILD_SERVER=TRUE -DBUILD_CLIENT=TRUE -DBUILD_WORLD_EDIT=TRUE
    make
else
    echo "Building in $1 mode."
    # Build projet in build type of argument
    cmake . -DCMAKE_BUILD_TYPE=$1 -DBUILD_SERVER=TRUE -DBUILD_CLIENT=TRUE -DBUILD_WORLD_EDIT=TRUE
    make
fi

# Copy data
sh CopyClientData.sh
sh CopyServerData.sh
