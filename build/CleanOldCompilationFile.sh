#!/bin/sh

echo Deleting old compilations file...

rm CMakeCache.txt 2> /dev/null
rm cmake_install.cmake 2> /dev/null
rm Makefile 2> /dev/null

rm -rf CMakeFiles/ 2> /dev/null

echo Done.
