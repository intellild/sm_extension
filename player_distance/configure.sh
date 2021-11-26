#!/bin/zsh

mkdir build
pushd build
python3 ../configure.py --hl2sdk-root ../../ --mms-path ../../metamod-source --sm-path ../../sourcemod -s csgo --enable-optimize
popd
