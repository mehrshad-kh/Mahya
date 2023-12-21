#!/bin/zsh

qt_version='6.5.2'

cmake -S . -B build -GNinja -DCMAKE_PREFIX_PATH=~/Qt/${qt_version}/macos
