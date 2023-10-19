CC=gcc
CFLAGS=-I.
DEPS = Config.h FileManagement.h FindFiles.h GlobToRegex.h main.h UpdateFiles.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: Config.o FileManagement.o FindFiles.o GlobToRegex.o main.o UpdateFiles.o
	$(CC) -o mysync Config.c FileManagement.c FindFiles.c GlobToRegex.c main.c UpdateFiles.c