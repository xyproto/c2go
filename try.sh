#!/bin/sh
if [ -n "${1+x}" ]; then
  X=`basename $1 .c`
  ../c2go.py $X.c > $X.go
  go fmt $X.go
  go build -o $X $X.go
else
  echo "No filename given"
fi
