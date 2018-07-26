import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

data_file = open("data.txt", "r")
start_mem_file = open("begin.txt", "r")
end_mem_file = open("end.txt", "r")

data = []
memberships_start = []
memberships_end = []

for line in data_file:
    data.append([float(x) for x in line.strip().split(" ")])

for line in start_mem_file:
    memberships_start.append([float(x) for x in line.strip().split(" ")])

for line in end_mem_file:
    memberships_end.append([float(x) for x in line.strip().split(" ")])

data = np.array(data)
memberships_start = np.array(memberships_start)
memberships_end = np.array(memberships_end)

memberships = [x.argmax() for x in memberships_end]

ax = plt.axes(projection='3d')

if (len(data.T) == 1):
    ax.scatter(data.T[0], c=memberships)
elif (len(data.T) == 2):
    ax.scatter(data.T[0], data.T[1], c=memberships)
else:
    ax.scatter(data.T[0], data.T[1], data.T[2], c=memberships)

plt.show()