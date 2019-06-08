#!/bin/bash

## This script launches a Docker container, mounting this project inside
##       allowing us to play around w/ system settings safely

source ./change_dir.sh
change_dir

docker run \
	-it --privileged \
	--name c-container \
	--mount type=bind,source="$(pwd)"/,target=/c-container \
	ubuntu:latest

# need `--privileged` to change hostname etc
