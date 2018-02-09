BINNAME = gol
OUT = bin/$(BINNAME)
CC = gcc
SRCDIR = ./src
OUTDIR = ./bin
CFLAGS = -g -Wall
LFLAGS = -lncurses
IFLAGS = ./include

all:
	$(CC) $(CFLAGS) $(SRCDIR)/* -o $(OUT) -I $(IFLAGS) $(LFLAGS)
	if [ ! -f $(BINNAME) ]; then ln -s -T  $(OUT) $(BINNAME); fi
