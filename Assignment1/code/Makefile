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
	python3 plotter.py m1.csv
	python3 plotter.py m2.csv
	python3 plotter.py m3.csv
	python3 plotter.py m4.csv
 
run1:
	g++ Method1.cpp -o Method1 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method1
	python3 plotter.py m1.csv

run2:
	g++ Method2.cpp -o Method2 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method2
	python3 plotter.py m2.csv

run3:
	g++ Method3.cpp -o Method3 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method3
	python3 plotter.py m3.csv

run4:
	g++ Method4.cpp -o Method4 -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Method4
	python3 plotter.py m4.csv

baseline:
	g++ Baseline.cpp -o Baseline -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./Baseline

incor:
	g++ InputCor.cpp -o InputCor -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./InputCor

dense:
	g++ DenseFlow.cpp -o DenseFlow -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./DenseFlow

sparse:
	g++ SparseFlow.cpp -o SparseFlow -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./SparseFlow

extra:
	g++ DenseFlow.cpp -o DenseFlow -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./DenseFlow
	g++ SparseFlow.cpp -o SparseFlow -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./SparseFlow

runextra:
	g++ DenseFlow.cpp -o DenseFlow -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./DenseFlow $(FILENAME)
	g++ SparseFlow.cpp -o SparseFlow -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	./SparseFlow $(FILENAME)


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
	python3 plotter.py m1.csv
	python3 plotter.py m2.csv
	python3 plotter.py m3.csv
	python3 plotter.py m4.csv

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
	g++ DenseFlow.cpp -o DenseFlow -pthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ SparseFlow.cpp -o SparseFlow -pthread -std=c++11 `pkg-config --cflags --libs opencv`

plot:
	python3 plotter.py m1.csv
	python3 plotter.py m2.csv
	python3 plotter.py m3.csv
	python3 plotter.py m4.csv

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
	rm runtime_vs_parameter_m4.png
	rm runtime_vs_parameter_m3.png
	rm runtime_vs_parameter_m2.png
	rm runtime_vs_parameter_m1.png
	rm runtime_vs_error_m4.png
	rm runtime_vs_error_m3.png
	rm runtime_vs_error_m2.png
	rm runtime_vs_error_m1.png
	rm error_vs_parameter_m4.png
	rm error_vs_parameter_m3.png
	rm error_vs_parameter_m2.png
	rm error_vs_parameter_m1.png
	rm dense.csv
	rm extra.txt

help:
	cat README.md

install:
	sudo apt-get install python3-matplotlib
	sudo apt install python3-pip
	pip3 install numpy
