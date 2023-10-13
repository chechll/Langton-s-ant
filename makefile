all: client server

clear:
	rm -f *.o *~ client server

client: client.cpp client.h
	g++ client.cpp -o client

server: server.cpp server.h
	g++ server.cpp -o server

.PHONY: all clear
