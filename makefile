CC    = gcc
FILE = demo.c
GTK_LIBS = $(shell pkg-config --libs --cflags gtk+-2.0)

all:	
	$(CC) -o demo $(FILE) $(GTK_LIBS)	
