SRCS=$(wildcard *.c)
GOSRC=$(SRCS:.c=.go)
APPS=$(SRCS:.c=)

all:
	$(foreach x,$(APPS),./c2go.py $(x).c > $(x).go;)
	$(foreach x,$(APPS),gofmt -w $(x).go;)
	$(foreach x,$(APPS),6g $(x).go;)
	$(foreach x,$(APPS),6l -o $(x) $(x).6;)

clean:
	rm -f *.6 lextab.py yacctab.py $(APPS) $(GOSRC) core
