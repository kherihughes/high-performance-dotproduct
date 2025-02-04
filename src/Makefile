CC=gcc
CFLAGS=-O3 -march=native -ffast-math -funroll-loops -ftree-vectorize
BLAS_CFLAGS=-I/usr/include/x86_64-linux-gnu
BLAS_LIBS=-lopenblas -lpthread

all: c1/dp1 c2/dp2 c3/dp3

c1/dp1: c1/dp1.c
	$(CC) $(CFLAGS) -o $@ $< $(BLAS_LIBS)

c2/dp2: c2/dp2.c
	$(CC) $(CFLAGS) -o $@ $< $(BLAS_LIBS)

c3/dp3: c3/dp3.c
	$(CC) $(CFLAGS) $(BLAS_CFLAGS) -o $@ $< $(BLAS_LIBS)

clean:
	rm -f c1/dp1 c2/dp2 c3/dp3 */*.txt

.PHONY: all clean 