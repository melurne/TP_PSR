CC=gcc
CFLAGS=-w -Wall -Wextra -l pthread

exo1: exo1.c
	$(CC) $(CFLAGS) -o $@ $<

exo2: exo2.c
	$(CC) $(CFLAGS) -o $@ $<

exo3: exo3.c
	$(CC) $(CFLAGS) -o $@ $<

