FILENAME="empty.jpg"

default:
	g++ A1_density.cpp -o A1_density -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./A1_density 
	python3 plotter.py
 
build:
	g++ A1_density.cpp -o A1_density -pthread -std=c++11 `pkg-config --cflags --libs opencv`

run:
	./A1_density $(FILENAME)

clean:
	rm A1_density

cleanall:
	rm A1_density
	rm moving.csv
	rm stationary.csv
	rm stationary.png

help:
	cat README.md
