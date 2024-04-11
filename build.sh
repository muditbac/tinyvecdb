#!/bin/bash

# Check if the first argument is --debug
if [ "$1" == "--debug" ]; then
    DEBUG_FLAG="-DDEBUG"
else
    DEBUG_FLAG=""
fi


g++ -std=c++11 -O3 -Xpreprocessor -fopenmp $DEBUG_FLAG \
    src/main.cpp \
    src/metrics.cpp \
    src/nsw.cpp \
    src/bfnn.cpp \
    src/commons.cpp -lomp -L/usr/local/opt/libomp/lib


./a.out && rm a.out