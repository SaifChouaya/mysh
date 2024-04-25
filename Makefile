CC=gcc
CFLAGS=-Wall -Wextra -g

mysh: mysh.c test.c
	$(CC) $(CFLAGS) -o mysh mysh.c test.c

run: mysh
	./mysh

open: mysh
ifeq ($(shell uname), Darwin)
	open mysh.c
else
	xdg-open mysh.c
endif

test: test.c
	$(CC) $(CFLAGS) -o test test.c
	./test

clean:
	rm -f mysh test