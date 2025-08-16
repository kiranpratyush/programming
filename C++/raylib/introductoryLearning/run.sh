#!/bin/bash

# Source file
SRC="loadImages.cc"

# Output executable
OUT="loadImages.out"

# Compile
g++ "$SRC" -o "$OUT" -lraylib -lm -ldl -lrt -lGL -pthread

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation succeeded. Running $OUT..."
    ./"$OUT"
else
    echo "Compilation failed."
fi
