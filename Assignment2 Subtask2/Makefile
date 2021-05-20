.PHONY: server client
TOT=50
ONCE=500


all:
	g++ Pathfinding.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o Pathfinding
	./Pathfinding

build: 
	g++ Pathfinding.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o Pathfinding

run:
	./Pathfinding $(TOT) $(ONCE)
