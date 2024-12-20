from math import *
inp = (input().split(" "))
n = -2
bottom = 0
start = eval(inp[-1]) * eval(inp[-2])
mid = start
a = 1
for i in range(len(inp)-3):
    n -= 1
    mid = eval(inp[n]) * (mid + a)
    beside = eval(inp[n + 2])
    num = 1 / (beside + bottom)
    b = beside + bottom
    a = a* b
    bottom = num
mid = mid + a
below = int(mid)
n = 0
bottom = 0
a = 1
for j in range(len(inp)-2):
    n -= 1
    beside = eval(inp[n])
    num = 1 / (beside + bottom)
    b = beside + bottom
    a = a* b
    bottom = num
above = int(a + below * eval(inp[0]))
n = 0
m = 0
for i in range(100):
    m += 1
    for j in range(ceil(above / below) * 100):
        n += 1
        if n / m == above / below:
            above = n
            below = m
print(above,below)
