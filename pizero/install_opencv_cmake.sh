#!/bin/bash

# TODO: Throw in commands to install compilers, upgrade apt-get and install the vnc server.
# also something about expanding the raspberry pi filesystem to take up the whole sd card

sudo apt-get install libopencv-dev python-opencv

sudo apt-get install software-properties-common
sudo add-apt-repository ppa:george-edison55/cmake-3.x
sudo apt-get update
sudo apt-get install cmake
