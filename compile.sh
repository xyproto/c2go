#!/bin/sh
cd works
for f in *.c; do
  x=`basename $f .c`
  echo $x
  echo ../c2go.py $x.c \> $x.go
  ../c2go.py $x.c > $x.go
  rm -f lextab.py yacctab.py
  echo go fmt $x.go
  go fmt $x.go
  echo go build -o $x $x.go
  go build -o $x $x.go && echo ok
done
cd ..
