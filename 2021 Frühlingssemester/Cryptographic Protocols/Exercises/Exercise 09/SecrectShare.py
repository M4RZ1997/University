from Cryptodome.Random import random
from Cryptodome.Util import number
import random


# object to represent every S_i with its index and value (s.t. indices are available for reconstruction)
class Share:
    def __init__(self, index, value):
        self.index = index
        self.value = value


def polynomialValue(value, coefficients, prime):
    result = 0
    for x in range(1, len(coefficients)):
        val = (coefficients[x] * (value ** x % prime)) % prime
        result = (result + val) % prime

    result = (result + coefficients[0]) % prime
    return result


def share(polynomialDegree, noParticipants, s, q):
    if polynomialDegree >= noParticipants:
        print("the degree T of the polynomial should always be smaller than the number of participents N")
        return []
    else:
        coeffs = [s]
        for x in range(1, polynomialDegree + 1):
            c = random.randint(0, q)
            coeffs.append(c)
        
        for i, v in enumerate(coeffs):
            print("Coefficient " + str(i) + " is " + str(v) + "\n")

        values = []
        for y in range(1, noParticipants + 1):
            val = polynomialValue(y, coeffs, q)
            values.append(Share(y, val))

        return values


def reconstruct(list_, q):
    result = 0
    for x in list_:
        i = x.index
        copyList = list(list_)
        index = copyList.index(x)
        copyList.pop(index)
        lagrangeX = 1
        print("Share " + str(x.index) + " is " + str(x.value) + "\n")
        for y in copyList:
            val = (y.index * number.inverse(((y.index - i) % q), q)) % q
            lagrangeX = (lagrangeX * val) % q

        print("Lagrange of " + str(x.index) + " is " + str(lagrangeX) + "\n")
        v = (lagrangeX * x.value) % q
        result = (result + v) % q
        print("Lagrange of " + str(x.index) + " mult is " + str(result) + "\n")
    return result


# random prime with 2000 bits
q = number.getPrime(2000)
print("Prime is " + str(q) + "\n")

# generate secret s
s = random.randint(0, q)
print("Secret is " + str(s) + "\n")

n = number.getRandomRange(10, 100)
print("Number of participants " + str(n) + "\n")

t = number.getRandomRange(1,n)
print("Polynomial Degree " + str(t) + "\n")
           
n_list = share(t, n, s, q)
print("SHARES: \n")
for x in n_list:
    print("Share " + str(x.index) + " is " + str(x.value) + "\n")


sublist = random.sample(n_list, t+1)

print("SUB LIST: \n")
for x in sublist:
    print("Share " + str(x.index) + " is " + str(x.value) + "\n")

secret = reconstruct(sublist, q)

print("Is the reconstructed secret equal to the original : " + str(secret == s) + "\n")

