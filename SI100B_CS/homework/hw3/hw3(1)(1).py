import numpy as np
from math import *
matric = np.array(eval(input()))
matric1 = np.rot90(matric , k=3)
print(matric1.tolist())
if len(matric)%2 == 0:
    x = ceil((len(matric)-1)/2)
if len(matric)%2 != 0:
    x = ceil((len(matric)-1)/2)+1
if len(matric[0])%2 == 0:
    y = ceil((len(matric[0])-1)/2)
if len(matric[0])%2 != 0:
    y = ceil((len(matric[0])-1)/2)+1
matric2 = matric[:x,:y]
print(matric2.tolist())
matric3 = np.flip(matric,1)
print(matric3.tolist())
matric4 = np.ones((len(matric),len(matric[0])),dtype=int)
for i in range(len(matric)):
    for j in range(len(matric[0])):
        matric4[i][j] = 255 - matric[i][j]
print(matric4.tolist())
