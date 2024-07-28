#!/bin/bash

# Determine the platform
PLATFORM=$(uname)

# Run the appropriate executable
if [[ "$PLATFORM" == *"MINGW"* || "$PLATFORM" == *"CYGWIN"* ]]; then
    # If on Windows (Git Bash or Cygwin)
    ./build/bin/engine.exe
else
    # On Unix-like systems (Linux, macOS, etc.)
    ./build/bin/engine
fi
