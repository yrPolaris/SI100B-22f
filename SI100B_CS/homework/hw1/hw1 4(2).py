from math import *
inp = (input().split(" "))
mid_result = 0
x = 0
m, n = 0, 0
for i in range(len(inp) - 1):
    x -= 1
    mid_result = 1 / (eval(inp[x]) + mid_result)
result = eval(inp[0]) + mid_result
for i in range(1, 100):
    m += 1
    for j in range(1, ceil(result) * 100):
        n += 1
        if n / m == result:
            print(n , m)
            result = 0
        
