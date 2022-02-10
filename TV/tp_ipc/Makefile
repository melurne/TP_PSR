CC=gcc
CFLAGS=-Wall -Wextra
LFLAGS=-pthread

pong: pong.c
	$(CC) $(CFLAGS) $< -o $@

ping: ping.c
	$(CC) $(CFLAGS) $< -o $@ 

server: server.c
	$(CC) $(CFLAGS) $< -o $@ $(LFLAGS)

client: client.c
	$(CC) $(CFLAGS) $< -o $@ $(LFLAGS)

clean: 
	rm -f pong ping
