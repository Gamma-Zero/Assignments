import numpy as np
import matplotlib.pyplot as plt

data=np.genfromtxt("moving.csv",delimiter=",",names=["x","y"])
plt.plot(data['x'],data['y'],label='Moving Density')
plt.xlabel('Time')
plt.ylabel('Density')


data=np.genfromtxt("stationary.csv",delimiter=",",names=["x","y"])
plt.plot(data['x'],data['y'], label='Queue Density')
plt.legend()
plt.savefig("stationary.png")

