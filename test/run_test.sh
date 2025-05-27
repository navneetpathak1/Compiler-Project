#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Run CMake and make
cmake ..
make

# Run the test
./gehu_test 