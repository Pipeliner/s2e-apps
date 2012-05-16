CFLAGS+=-I../include -std=c99 -m32 -g -O0 -fno-stack-protector
LDFLAGS+=-m32
SRC=$(wildcard *.c)
PROGS=$(subst .c,,$(SRC))

all: $(PROGS) ../include/s2e.h

clean:
	rm $(PROGS)
