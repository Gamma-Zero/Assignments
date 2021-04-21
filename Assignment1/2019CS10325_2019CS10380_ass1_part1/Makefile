FILENAME="empty.jpg"

default:
	g++ A1.cpp -o A1 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./A1

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
