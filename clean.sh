#!/bin/sh

cd works
for f in *.c; do
  x=`basename $f .c`
  rm -f $x $x.go
done
cd ..

cd doesnotworkyet
for f in *.c; do
  x=`basename $f .c`
  rm -f $x $x.go
done
cd ..
