FILENAME="empty.jpg"

default:
	g++ Method1.cpp -o Method1 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method1
	g++ Method2.cpp -o Method2 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method2
	g++ Method3.cpp -o Method3 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method3
	g++ Method4.cpp -o Method4 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method4
 
run1:
	g++ Method1.cpp -o Method1 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method1

run2:
	g++ Method2.cpp -o Method2 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method2

run3:
	g++ Method3.cpp -o Method3 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method3

run4:
	g++ Method4.cpp -o Method4 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method4

baseline:
	g++ Baseline.cpp -o Baseline -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Baseline

incor:
	g++ InputCor.cpp -o InputCor -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./InputCor

fresh:
	g++ InputCor.cpp -o InputCor -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./InputCor
	g++ Baseline.cpp -o Baseline -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Baseline
	g++ Method1.cpp -o Method1 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method1
	g++ Method2.cpp -o Method2 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method2
	g++ Method3.cpp -o Method3 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method3
	g++ Method4.cpp -o Method4 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method4


run:
	g++ InputCor.cpp -o InputCor -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./InputCor $(FILENAME)
	g++ Baseline.cpp -o Baseline -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Baseline $(FILENAME)
	g++ Method1.cpp -o Method1 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method1 $(FILENAME)
	g++ Method2.cpp -o Method2 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method2 $(FILENAME)
	g++ Method3.cpp -o Method3 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method3 $(FILENAME)
	g++ Method4.cpp -o Method4 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method4 $(FILENAME)

build:
	g++ InputCor.cpp -o InputCor -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ Baseline.cpp -o Baseline -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ Method1.cpp -o Method1 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ Method2.cpp -o Method2 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ Method3.cpp -o Method3 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ Method4.cpp -o Method4 -pthread -std=c++11 `pkg-config --cflags --libs opencv`

clean:
	rm InputCor
	rm Baseline
	rm Method1
	rm Method2
	rm Method3
	rm Method4

cleanall:
	rm InputCor
	rm Baseline
	rm Method1
	rm Method2
	rm Method3
	rm Method4
	rm cor.csv
	rm stationary.csv
	rm m1.csv
	rm m2.csv
	rm m3.csv
	rm m4.csv

help:
	cat README.md

install:
	sudo apt-get install python3-matplotlib
	sudo apt install python3-pip
	pip3 install numpy
