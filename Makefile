CC=gcc
LIBS=-lncurses
FLAGS=$(LIBS)
DEBUG=-ggdb3 -pg
trg=life

all:	$(trg)

$(trg):	life.o
	$(CC) $< -o $@ $(FLAGS)

life.o:	game.c macros.h
	$(CC) -c -o $@ $<

clean:
	rm -f *.o

