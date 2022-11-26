#! /bin/sh

cmake -S . -B build/ -G "MinGW Makefiles";
cd ./build/;
mingw32-make

echo ""
read -p "Press [ENTER] key to exit ..."
