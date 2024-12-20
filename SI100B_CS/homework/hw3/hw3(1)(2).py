import numpy as np
from math import *
matric = np.array(eval(input()))
matric1 = matric[::2,::2]
print(matric1.tolist())
matric2 = np.zeros((len(matric)-2,len(matric[0])-2),dtype=int)
for i in range(len(matric)-2):
    for j in range(len(matric[0])-2):
        average = int(np.mean(matric[i:i+3,j:j+3]))
        matric2[i][j] = average
print(matric2.tolist())
matric3 = np.zeros((1,16),dtype = int)
for i in range(len(matric)):
    for j in range(len(matric[0])):
        x = matric[i][j]//16
        y = matric3[0][x]
        y += 1
        matric3[0][x] = y
print(matric3[0].tolist())
            
