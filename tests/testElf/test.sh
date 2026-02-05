#!/usr/bin/env bash

set -e

gcc tests/testElf/basic-c.c -m32 -o test.o
gcc tests/testElf/printHeaders.c -m32 -o headerPrint.o

echo "~~~~~~~Header Print Start~~~~~~~~~~~~~"

./headerPrint.o

echo "~~~~~~~Header Print End~~~~~~~~~~~~~"

readelf -h -S test.o

rm test.o
rm headerPrint.o
