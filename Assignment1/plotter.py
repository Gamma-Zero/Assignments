import numpy as np
import matplotlib.pyplot as plt

data=np.genfromtxt("dynamic.csv",delimiter=",",names=["x","y"])
plt.plot(data['x'],data['y'], label='Queue Density')
plt.legend()
plt.savefig("dynamic.png")

