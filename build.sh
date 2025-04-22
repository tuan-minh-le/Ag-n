#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Run CMake
cmake ..

# Build the project
make -j$(nproc)

# Copy assets
cp -r ../res ./

echo "Build complete! Run ./Ag-n to start the game."
