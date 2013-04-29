#!/bin/sh
if [ -n "${1+x}" ]; then
  X=`basename $1 .c`
  ../c2go.py $X.c > $X.go
  gofmt -w $X.go
  6g $X.go
  6l -o $X $X.6
else
  echo "No filename given"
fi

