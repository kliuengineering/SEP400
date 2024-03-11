CC = g++
CFLAGS = -Wall
FILES = server.cpp
FILES += client.cpp

server: server.cpp
	$(CC) $(CFLAGS) $^ -o $@

client: client.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean: 
	rm -f server client *.o 

all: server client
