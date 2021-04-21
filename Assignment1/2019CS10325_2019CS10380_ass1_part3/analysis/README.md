plotter.py : a plotter which takes csv file as commandline input. It generates 3 graphs from the csv passed to it (runtime vs parameter, error vs parameter, runtime vs error). Run as python3 plotter.py <filename>.
cor.csv : 4 coordinates to run perspective correction.
stationary.csv : baseline values for method 1,2,3,4.

Extra Credit:
dense.csv : baseline values for dynamic density.
extra.txt : contains information about sparse and dense optical flow (Error and Runtimes).

Method Files:
Method x contains mx.csv, error_vs_parameter_mx.png, runtime_vs_error_mx.png, runtime_vs_parameter_mx.png where x is the method number.
mx.csv : Error and Runtime(in seconds) for various Parameters are stored as Error,Runtime,Parameter.
error_vs_parameter_mx.png : Error vs Parameter graph generated from mx.csv.
runtime_vs_error_mx.png : Runtime vs Error graph generated from mx.csv.
runtime_vs_parameter_mx.png : Runtime vs Parameter graph generated from mx.csv.
