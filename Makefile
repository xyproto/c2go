all: test hello power2 homework1

test:
	./c2go.py test.c > test.go
	6g test.go
	6l -o test test.6

hello:
	./c2go.py hello.c > hello.go
	6g hello.go
	6l -o hello hello.6

power2:
	./c2go.py power2.c > power2.go
	6g power2.go
	6l -o power2 power2.6

homework1:
	./c2go.py homework1.c > homework1.go
	6g homework1.go
	6l -o homework1 homework1.6

clean:
	rm -f *.6 test test.go hello hello.go power2 power2.go homework1 homework.go lextab.py yacctab.py
