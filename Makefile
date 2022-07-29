CC=gcc
CFLAGS=-I.
DEPS = wordList.h
OBJ = wordList.o my-grep.o 

all: my-grep my-cat my-unzip my-zip reverse wish

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

my-grep: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

my-cat:
	$(CC) -o my-cat my-cat.c -Wall -Werror

my-unzip:
	$(CC) -o my-unzip my-unzip.c -Wall -Werror

my-zip:
	$(CC) -o my-zip my-zip.c -Wall -Werror

reverse:
	$(CC) -o reverse reverse.c -Wall -Werror

wish:
	$(CC) -o wish wish.c -Wall -Werror

clean:
	rm -f reverse
	rm -f $(OBJ)
	rm -f my-unzip
	rm -f my-zip
	rm -f my-cat
	rm -f wish