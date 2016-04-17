
CC=gcc
CFLAGS=-I$(HOME)/src/libgxkit
LDFLAGS=-L$(HOME)/src/libgxkit

LIBS=-lgxkit -lmatrix

imgconvolute : imgconvolute.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)
