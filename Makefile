# $@ - Target name
# $^ - Target dependencies

CC = gcc
CFLAGS = -Wall -Wpedantic

first:
	echo "Joe Rules! Take a look at the make file to view make options."

release: src/main.c src/util.c
	$(CC) $^ $(CFLAGS) -lncurses -o bin/ttt_$@

debug: src/main.c src/util.c
	$(CC) $^ $(CFLAGS) -g3 -lncurses -o bin/ttt_$@
