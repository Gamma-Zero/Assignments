FILENAME="empty.jpg"

default:
	g++ A1_density.cpp -o A1_density -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./A1_density 
	python3 plotter.py
 
build:
	g++ A1.cpp -o A1 -pthread -std=c++11 `pkg-config --cflags --libs opencv`

run:
	./A1 $(FILENAME)

clean:
	rm A1

cleanall:
	rm A1
	rm Cropped.jpg
	rm Transformed.jpg

help:
	cat README.md
