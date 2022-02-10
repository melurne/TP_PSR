CC=gcc
CFLAGS=-Wall -Wextra

pong: pong.c
	$(CC) $(CFLAGS) $< -o $@

ping: ping.c
	$(CC) $(CFLAGS) $< -o $@ 

server: server.c
	$(CC) $(CFLAGS) $< -o $@ 


clean: 
	rm -f pong ping
