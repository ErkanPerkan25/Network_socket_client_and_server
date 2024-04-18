DEBUG = -g
CXXFLAGS = $(DEBUG) -Wall -ansi -pedantic -std=c++20
LDFLAGS = -c

CXX=g++ 

OBJS = client.o server.o

.PHONY: all clean

all: client server

client: client.cpp
	$(CXX) $(CXXFLAGS) -o client client.cpp 

server: server.cpp
	$(CXX) $(CXXFLAGS) -o server server.cpp -lpthread 

clean:
	/bin/rm -rf *~ $(OBJS) server client
