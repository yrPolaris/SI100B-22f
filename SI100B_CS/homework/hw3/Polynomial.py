class Polynomial:
    #################### Task1 #######################
    def __init__(self, coeffs):
        self.coeffs = coeffs

    def __str__(self):
        item = []
        for i in range(len(self.coeffs)):
            if i == 0:
                item.append(str(self.coeffs[i]))
            if i == 1:
                item.append(str(self.coeffs[i])+"x")
            if i >= 2:
                item.append(str(self.coeffs[i])+"x^"+str(i))
        polynomial = " + ".join(item)
        return polynomial

    def deg(self):
        deg = len(self.coeffs)-1
        return deg

    #################### Task2 #######################
    def __neg__(self):
        anti_item = []
        for i in range(len(self.coeffs)):
            if i == 0:
                anti_item.append(str(-self.coeffs[i]))
            if i == 1:
                anti_item.append(str(-self.coeffs[i])+"x")
            if i >= 2:
                anti_item.append(str(-self.coeffs[i])+"x^"+str(i))
        anti_polynomial = " + ".join(anti_item)
        return anti_polynomial

    def __add__(self, other):
        add_coeffs = []
        if len(self.coeffs) > len(other.coeffs):
            for i in range(len(self.coeffs)-len(other.coeffs)):
                other.coeffs.append(0)
        if len(self.coeffs) < len(other.coeffs):
            for i in range(len(other.coeffs)-len(self.coeffs)):
                self.coeffs.append(0)
        for i in range(max(len(self.coeffs),len(other.coeffs))):
            add_coeffs.append(self.coeffs[i] + other.coeffs[i])
        return Polynomial(add_coeffs)

    def __sub__(self, other):
        sub_coeffs = []
        if len(self.coeffs) > len(other.coeffs):
            for i in range(len(self.coeffs)-len(other.coeffs)):
                other.coeffs.append(0)
        if len(self.coeffs) < len(other.coeffs):
            for i in range(len(other.coeffs)-len(self.coeffs)):
                self.coeffs.append(0)
        for i in range(max(len(self.coeffs),len(other.coeffs))):
            sub_coeffs.append(self.coeffs[i] - other.coeffs[i])
        while sub_coeffs[-1] == 0 and len(sub_coeffs) > 1:
            sub_coeffs.pop(-1)
        return Polynomial(sub_coeffs)
    
    def __mul__(self, other):
        mul_coeffs = []
        for i in range(len(self.coeffs)+len(other.coeffs)-1):
            mul_coeffs.append(0)
        for j in range(len(self.coeffs)):
            for k in range(len(other.coeffs)):
                mul_coeffs[j+k] = self.coeffs[j]*other.coeffs[k] + mul_coeffs[j+k]
        return Polynomial(mul_coeffs)

    def evaluate(self, x):
        num = []
        for i in range(len(self.coeffs)):
            num.append(self.coeffs[i]*(x**i))
        eva = sum(num)
        return eva
            
    
    #################### Task3 #######################
    def derivate(self, m):
        for j in range(m):
            for i in range(len(self.coeffs)):
                self.coeffs[i] = self.coeffs[i]*i
            if len(self.coeffs) != 1 and self.coeffs[0] == 0:
                self.coeffs.pop(0)
            if len(self.coeffs) == 1 and self.coeffs[0] == 0:
                break
        return Polynomial(self.coeffs)

    def integral(self,m):
        for i in range(m):
            for i in range(len(self.coeffs)):
                self.coeffs[i] = self.coeffs[i]/(i+1)
            if len(self.coeffs) != 1 and self.coeffs[0] != 0:
                self.coeffs.insert(0,0.0)
            if len(self.coeffs) == 1 and self.coeffs[0] == 0:
                break
        for i in range(len(self.coeffs)):
            self.coeffs[i] = "{:.1f}".format(self.coeffs[i])
        return Polynomial(self.coeffs)
            
    
    def definite_integral(self,m,x1,x2):
        num1 = []
        num2 = []
        for i in range(m):
            for i in range(len(self.coeffs)):
                self.coeffs[i] = self.coeffs[i]/(i+1)
            if len(self.coeffs) != 1 and self.coeffs[0] != 0:
                self.coeffs.insert(0,0.0)
            if len(self.coeffs) == 1 and self.coeffs[0] == 0:
                break
        for i in range(len(self.coeffs)):
            num1.append(self.coeffs[i]*(x1**i))
            num2.append(self.coeffs[i]*(x2**i))
        eva = sum(num1) - sum(num2)
        ans = eval("{:.1f}".format(eva))
        return ans

########## DON'T MODIFY THE CODE BELOW ##########
if __name__ == "__main__":
    print(eval(input()))
