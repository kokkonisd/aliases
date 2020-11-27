CC = cc
CFLAGS = -Wall -g
PREFIX ?= /usr/local/bin

all : aliases

install : aliases
	install aliases $(PREFIX)/aliases

uninstall :
	rm -f $(PREFIX)/aliases

aliases : aliases.o
	$(CC) $(CFLAGS) aliases.o -o aliases

aliases.o : aliases.c
	$(CC) $(CFLAGS) -c aliases.c

aliases.c : aliases.h

clean :
	rm -rf aliases aliases.o *.dSYM

.PHONY : all install uninstall clean
