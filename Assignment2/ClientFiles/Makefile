.PHONY: server client rendering

rendering:
	g++ rendering.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o rendering
	./rendering

server:
	g++ server.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o server
	./server

client:
	g++ client.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o client
	./client
