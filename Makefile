CC = cc
CFLAGS = -Wall -g
VERSION = 1.0

all : aliases

install : aliases
	cp aliases /usr/local/bin/

uninstall :
	rm -f /usr/local/bin/aliases

aliases : aliases.o
	$(CC) $(CFLAGS) aliases.o -o aliases

aliases.o : aliases.c
	$(CC) $(CFLAGS) -c aliases.c

aliases.c : aliases.h

clean :
	rm -rf aliases aliases.o *.dSYM

.PHONY : all install uninstall clean
