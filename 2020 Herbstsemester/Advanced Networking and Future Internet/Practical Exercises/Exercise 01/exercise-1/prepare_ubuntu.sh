#!/bin/bash

sudo dpkg --add-architecture i386 && \
sudo apt-get update && \
sudo apt install ffmpeg wine
