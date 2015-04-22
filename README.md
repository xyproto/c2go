C2Go 0.1
========

C2Go is based on the c-to-c.py example from pycparser by Eli Bendersky, and uses pycparser extensively.

* Alexander Rødseth <rodseth@gmail.com>
* License: BSD

The C examples are from: http://www.cis.temple.edu/~ingargio/cis71/code/.


Requirements
------------

* pycparser
* ply

Improvements
------------

If you wish to help out, here is one way:

1. Enter the "doesnotworkyet" directory
1. Select a c file to work on
1. Type `../try.sh name`, where name is the filename before ".c"
1. Observe the error messages when the generated Go file doesn't compile
1. Edit the Go file to make it compile again, to know what to work towards
1. Edit c2go.py to make it generate a Go file that does compile for the given test case
1. Create a pull request

Pull requests are appreciated.


Changes
-------

* 2011-08-30 - Initial commit
* 2013-04-29 - Update for Go 1
* 2014-05-07 - Minor changes

