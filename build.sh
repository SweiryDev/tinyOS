#!/bin/bash

rm -f tinyos.img
make clean
make

if [ $? -eq 0 ]; then
    make clean
    echo "Build Successful"
else
    echo "Build Failed"
fi