CFLAGS+=-I/home/s2e/s2e/s2e/guest/include -std=c99 -m32
LDFLAGS+=-m32
PROGS=factorial simple fibonacci overlap fs ks

all: $(PROGS)

fibonacci: fibonacci.o
factorial: factorial.o
simple: simple.o
overlap: overlap.o
ks: ks.o
fs: fs.o

clean:
	rm $(PROGS) *.o
