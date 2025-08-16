#!/bin/bash

# Source file
SRC="main.cc"

# Output executable
OUT="main"

# Compile
g++ "$SRC" -o "$OUT" -lraylib -lm -ldl -lrt -lGL -pthread

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation succeeded. Running $OUT..."
    ./"$OUT"
else
    echo "Compilation failed."
fi
