FILENAME="empty.jpg"

default:
	g++ Method1.cpp -o Method1 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method1
	g++ Method2.cpp -o Method2 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method2
	g++ A1_method4.cpp -o A1_method4 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./A1_method4
	g++ A1_method3.cpp -o A1_method3 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./A1_method3
 
run1:
	g++ Method1.cpp -o Method1 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method1

run2:
	g++ Method1.cpp -o Method1 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method1

run3:
	g++ A1_method3.cpp -o A1_method3 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./A1_method3

run4:
	g++ A1_method4.cpp -o A1_method4 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./A1_method4

help:
	cat README.md

install:
	sudo apt-get install python3-matplotlib
	sudo apt install python3-pip
	pip3 install numpy
