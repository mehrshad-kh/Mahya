#!/bin/zsh

qt_version='6.5.2'

script_location="${0%/*}"

cmake -S "$script_location"/. \
  -B "$script_location"/build \
  -GNinja -DCMAKE_PREFIX_PATH=~/Qt/${qt_version}/macos -Wno-dev
