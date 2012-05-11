CFLAGS+=-I../include -std=c99 -m32 -g -O0 -fno-stack-protector
LDFLAGS+=-m32
SRC=$(wildcard *.c)
PROGS=$(subst .c,,$(SRC))
#PROGS=factorial simple fibonacci overlap fs ks equations divider divider-wrapper returner assert ab

all: $(PROGS)

clean:
	rm $(PROGS)
