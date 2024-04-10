#!/bin/bash

# Check if the first argument is --debug
if [ "$1" == "--debug" ]; then
    DEBUG_FLAG="-DDEBUG"
else
    DEBUG_FLAG=""
fi


g++ -std=c++11 -O3 $DEBUG_FLAG \
    src/main.cpp src/metrics.cpp src/nsw.cpp src/ds.cpp
./a.out && rm a.out