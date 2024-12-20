test = [1,2,3,4]
for i in test:
    if i == 2:
        test.remove(i)
        test.insert(0,i)
print(test)

