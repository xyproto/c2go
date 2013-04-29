#!/bin/sh

cd works
rm -f lextab.py yacctab.py
for f in *.c; do
  x=`basename $f .c`
  rm -f $x $x.go
done
cd ..

cd doesnotworkyet
rm -f lextab.py yacctab.py
for f in *.c; do
  x=`basename $f .c`
  rm -f $x $x.go
done
cd ..
