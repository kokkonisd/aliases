CC = gcc
CFLAGS = -Wall -Wextra
PREFIX ?= /usr/local/bin
DEBUG ?= 1

ifeq ($(shell uname -s), Darwin)
	CFLAGS += -DMACOS
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -g
else
	CFLAGS += -O3
endif

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
