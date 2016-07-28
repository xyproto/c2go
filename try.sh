#!/bin/sh
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [ ! -n "${1+x}" ]; then
  echo "No filename given"
  exit 1
else
  X=`basename $1 .c`
  if [ ! -f $X.c ]; then
    echo "Must be run from the same directory as $X.c"
    exit 1
  fi
  "$DIR/c2go.py" $X.c > $X.go
  go fmt $X.go
  go build -o $X $X.go
fi
