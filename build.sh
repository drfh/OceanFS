#!/bin/bash
#

cd _build
clear
cmake ..
make clean
make -j
make test
