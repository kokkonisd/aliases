#!/bin/bash

if [ $# != 1 ]; then
    echo "Usage: $0 <version>"
    exit;
fi

VERSION=$1

mkdir aliases-$VERSION/
make
cp aliases.c aliases-$VERSION/
cp aliases.h aliases-$VERSION/
cp Makefile aliases-$VERSION/
tar -cvzf aliases-$VERSION.tar.gz aliases-$VERSION/
rm -rf aliases-$VERSION/
make clean
