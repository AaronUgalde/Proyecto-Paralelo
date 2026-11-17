# Makefile - Capitulo 5: Serie de Fourier con openMPI (macOS)
CC      = mpicc
CFLAGS  = -Wall -Wextra -std=c11 -O2
LDLIBS  = -lm
NP     ?= 100
BIN     = fourier_mpi

all: $(BIN)

$(BIN): fourier_mpi.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

run: $(BIN)
	mpirun -np $(NP) --oversubscribe ./$(BIN)

clean:
	rm -f $(BIN) fourier_mpi_resultados.csv fourier_mpi_grafica.csv

.PHONY: all run clean
