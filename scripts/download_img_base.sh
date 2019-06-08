#!/bin/bash

## This script downloads the Ubuntu base filesystem
##		so that we can build an image for the container


cd /tmp
apt-get install wget
wget https://github.com/ericchiang/containers-from-scratch/releases/download/v0.1.0/rootfs.tar.gz
sha256sum rootfs.tar.gz
tar -zxf rootfs.tar.gz
mkdir rootfs/INSIDE_CONTAINER
rm rootfs.tar.gz
