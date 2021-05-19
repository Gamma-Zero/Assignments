.PHONY: server client

IP = 127.0.0.1
PORT = 8080

server:
	g++ Server.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o Server
client:
	g++ Client.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o Client
runserver:
	./Server $(IP) $(PORT)

runclient:
	./Client $(IP) $(PORT)
