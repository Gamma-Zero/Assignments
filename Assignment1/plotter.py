import numpy as np
import matplotlib.pyplot as plt

data=np.genfromtxt("moving.csv",delimiter=",",names=["x","y"])
plt.plot(data['x'],data['y'])
plt.xlabel('Framenum')
plt.ylabel('Dynamic Density')
plt.savefig("moving.png")

data=np.genfromtxt("stationary.csv",delimiter=",",names=["x","y"])
plt.plot(data['x'],data['y'])
plt.xlabel('Framenum')
plt.ylabel('Queue Density')
plt.savefig("stationary.png")

