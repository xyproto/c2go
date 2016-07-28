#!/bin/sh

cd works
rm -f lextab.py yacctab.py c2go.log
for f in *.c; do
  x=`basename $f .c`
  rm -f $x $x.go
done
cd ..

cd doesnotworkyet
rm -f lextab.py yacctab.py c2go.log
for f in *.c; do
  x=`basename $f .c`
  rm -f $x $x.go
done
cd ..
