#!/bin/bash
# install compiler for Travis CI

sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -qq

if [ "$CXX" = "clang++" ];
then
    sudo apt-get install -qq libstdc++-4.8-dev;
fi


if [ "$CXX" = "g++" ];
then
    sudo apt-get install -qq g++-4.8;
    export CXX="g++-4.8";
    export CC="gcc-4.8";
fi
