import numpy as np
import matplotlib.pyplot as plt
import sys

w=sys.argv[1]
data=np.genfromtxt(w,delimiter=",",names=["x","y","z"])
if (w=="m1.csv"):

	plot1 = plt.figure(1)
	plt.plot(data['x'],data['y'], label='Queue Density Sub-Sampling')
	plt.xlabel("Error")
	plt.ylabel("Runtime")
	plt.legend()
	plt.savefig("runtime_vs_error_m1.png")

	plot2 = plt.figure(2)
	plt.plot(data['z'],data['x'], label='Queue Density Sub-Sampling')
	plt.xlabel("Parameter")
	plt.ylabel("Error")
	plt.legend()
	plt.savefig("error_vs_parameter_m1.png")

	plot3 = plt.figure(3)
	plt.plot(data['z'],data['y'], label='Queue Density Sub-Sampling')
	plt.xlabel("Parameter")
	plt.ylabel("Runtime")
	plt.legend()
	plt.savefig("runtime_vs_parameter_m1.png")

elif (w=="m2.csv"):
	plot1 = plt.figure(1)
	plt.plot(data['x'],data['y'], label='Queue Density Resolution')
	plt.xlabel("Error")
	plt.ylabel("Runtime")
	plt.legend()
	plt.savefig("runtime_vs_error_m2.png")

	plot2 = plt.figure(2)
	plt.plot(data['z'],data['x'], label='Queue Density Resolution')
	plt.xlabel("Parameter")
	plt.ylabel("Error")
	plt.legend()
	plt.savefig("error_vs_parameter_m2.png")

	plot3 = plt.figure(3)
	plt.plot(data['z'],data['y'], label='Queue Density Resolution')
	plt.xlabel("Parameter")
	plt.ylabel("Runtime")
	plt.legend()
	plt.savefig("runtime_vs_parameter_m2.png")

elif (w=="m3.csv"):
	plot1 = plt.figure(1)
	plt.plot(data['x'],data['y'], label='Multithreading(Number of Splits)')
	plt.xlabel("Error")
	plt.ylabel("Runtime")
	plt.legend()
	plt.savefig("runtime_vs_error_m3.png")

	plot2 = plt.figure(2)
	plt.plot(data['z'],data['x'], label='Multithreading(Number of Splits)')
	plt.xlabel("Parameter")
	plt.ylabel("Error")
	plt.legend()
	plt.savefig("error_vs_parameter_m3.png")

	plot3 = plt.figure(3)
	plt.plot(data['z'],data['y'], label='Multithreading(Number of Splits)')
	plt.xlabel("Parameter")
	plt.ylabel("Runtime")
	plt.legend()
	plt.savefig("runtime_vs_parameter_m3.png")

else:
	plot1 = plt.figure(1)
	plt.plot(data['x'],data['y'], label='Multithreading(Number of Threads)')
	plt.xlabel("Error")
	plt.ylabel("Runtime")
	plt.legend()
	plt.savefig("runtime_vs_error_m4.png")

	plot2 = plt.figure(2)
	plt.plot(data['z'],data['x'], label='Multithreading(Number of Threads)')
	plt.xlabel("Parameter")
	plt.ylabel("Error")
	plt.legend()
	plt.savefig("error_vs_parameter_m4.png")

	plot3 = plt.figure(3)
	plt.plot(data['z'],data['y'], label='Multithreading(Number of Threads)')
	plt.xlabel("Parameter")
	plt.ylabel("Runtime")
	plt.legend()
	plt.savefig("runtime_vs_parameter_m4.png")





