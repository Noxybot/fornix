#!/bin/bash

cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build  --target install

if [ $? -eq 0 ]; then
    source run.sh
fi