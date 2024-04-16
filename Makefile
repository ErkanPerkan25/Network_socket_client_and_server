DEBUG = -g
OPTS = $(DEBUG) -Wall -ansi -pedantic -std=c++20

CC=g++ $(OPTS) -o

OBJS = client.o server.o

client: client.cpp
	$(CC) client

server: server.cpp
	$(CC) server -lpthread 

clean:
	/bin/rm -rf *~ $(OBJS) server client
