#!/bin/bash

mkdir -p build
cd build
make -j2

if [[ $? =~ 0 ]]
then
	./hello_gl
fi

cd ..