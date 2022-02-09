#!/bin/bash

mkdir -p build

cd build

for arch in i386 ppc
do
	echo $arch
	cmake -DDO_TESTS=off -DMAKE_DOCS=off -DCMAKE_OSX_ARCHITECTURES=$arch ..
	make clean
	make
	mv src/main/CircuitTester.app/Contents/MacOS/CircuitTester src/main/CircuitTester.app/Contents/MacOS/CircuitTester.$arch
done

#script="#!/bin/sh\npwd\n./CircuitTester.$(uname -m)"
#echo $script > src/main/CircuitTester.app/Contents/MacOS/CircuitTester
#chmod +x src/main/CircuitTester.app/Contents/MacOS/CircuitTester
