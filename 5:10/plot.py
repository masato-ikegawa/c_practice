import matplotlib.pyplot as plt
import numpy as np

x = np.arange(-10, 10, 0.1)
#x = np.arange(-3, 3, 0.1)
y = x**3 - 3*x**2 * np.sin(x) -1
plt.xlabel('X')
plt.ylabel('Y')
plt.grid()
plt.plot(x, y)