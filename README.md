C2Go 0.2
========

C2Go is based on the c-to-c.py example from pycparser by Eli Bendersky, and uses pycparser extensively.

The C examples are from: http://www.cis.temple.edu/~ingargio/cis71/code/.


Requirements
---

* python 2 or python 3
* pycparser
* ply


Installation
---

`c2go.py` can be installed as ie. `/usr/bin/c2go`.


Usage
---

The utility takes a filename as the first argument.

The parts of the C code that C2GO is unable to translate will be marked with a `/* C2GO: ? */` comment in the resulting output.


Changes
---

* 2011-08-30 - Initial commit
* 2013-04-29 - Update for Go 1
* 2014-05-07 - Minor changes
* 2015-11-12 - Minor fix


General information
---

* Alexander F Rødseth <xyproto@archlinux.org>
* License: BSD
* Pull requests are appreciated.
