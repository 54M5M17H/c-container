#!/bin/bash

source ./change_dir.sh
change_dir

# exit if compilation errors
gcc -o main.out main.c || exit 1
./main.out "$@"
