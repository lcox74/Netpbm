CC=gcc
CFLAGS=-lm -O3 -Wall
OUT=test

$(OUT): main.c netpbm.h
	$(CC) -o $(OUT) main.c $(CFLAGS)

