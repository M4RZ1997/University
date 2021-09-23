from Cryptodome.Random import random
from Cryptodome.Util import number
import random


def power(a, x, p):
    res = 1
    a = a % p
    if (a == 0):
        return 0
    while (x > 0):
        # Square if exponent is even
        if ((x & 1L) == 0):
            a = (a * a) % p
            x = x >> 1
        # Multiply if exponent is odd
        else:
            res = (res * a) % p
            x = x - 1
    return res

def randomsubgroup(q, p):
    while True:
        h = random.randint(2, p-1)
        g = power(h, (p-1)/q, p)
        if g != 1:
            break
    return g

def Parameters(qbits, pbits):
    while True:
        while True:
            q = random.getrandbits(qbits)
            if (number.isPrime(q)):
                break
        m = random.getrandbits(pbits - qbits - 1)
        p = m * q + 1
        if (number.isPrime(p)):
            break
    g = randomsubgroup(q, p)
    return (g, q, p)

# Taken from previous exercise
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
        print("The degree T of the polynomial should always be smaller than the number of participants N")
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
            val = (y.index * number.inverse(((y.index - i) % q),q)) % q
            lagrangeX = (lagrangeX * val) % q

        print("Lagrange of " + str(x.index) + " is " + str(lagrangeX) + "\n")
        v = (lagrangeX * x.value) % q
        result = (result + v) % q
        print("Lagrange of " + str(x.index) + " mult is " + str(result) + "\n")
    return result

# ElGamal encryption scheme
def KEYGEN(s, n, t, p, q, g):
    list = []
    print("g^x = " + str(g**s) + " and with mod p " + str((g**s) % p) + "\n")
    list.append(power(g,s,p))

    coeffs = []
    coeffs.append(s)
    for a in range(1,t+1):
        coeffs.append(random.randint(2,q-1))

    for i,v in enumerate(coeffs):
            print("Coeff " + str(i) + " is " + str(v) + "\n")

    for b in range(1,n+1):
        list.append(Share(b,polynomialValue(b,coeffs,q)))
    
    return list

def ENC(pk, m, g, p, q):
    r = random.randint(2, q-1)
    print("little r is " + str(r) + "\n")
    R = power(g,r,p)
    C = (m*power(pk,r,p)) % p
    return (R,C)

def DEC(pk, share, R, C, p, q):
    d = Share(share.index,power(R,share.value,p))
    return d

def RECOVER(pk, d_list, R, C, p, q):
    lag_list = []

    for x in d_list:
        lag = 1
        for y in d_list:
            if y.index == x.index:
                continue
            lag = (lag * (y.index * (number.inverse(y.index - x.index,q)) % q)) % q
        print("D_list " + str(x.index) + " hat Wert " + str(x.value) + " mit lagrange " + str(lag) + "\n")
        lag_list.append(lag)

    VAL = 1

    for z in range(0,len(d_list)):
        VAL = (VAL * power(d_list[z].value, lag_list[z], p)) % p

    print("Restored R^x is : " + str(VAL) + "\n")
    return (C * number.inverse(VAL,p)) % p


#################################################################################################
(g, q, p) = Parameters(15, 30)     # For use in practice

print("Prime is " + str(q) + "\n")

## get secret
s = random.randint(2,q-1) ## number.getRandomRange(0,q)
print("Secret is " + str(s) + "\n")

n = 5##number.getRandomRange(10,100)
print("Number of participants " + str(n) + "\n")

t = number.getRandomRange(1,n)
print("Degree of polynomial " + str(t) + "\n")

lists = KEYGEN(s,n,t,p,q,g)
print("public key: " + str(lists[0]) + "\n")
for c in range(1,n+1):
    print("Share " + str(c) + " has index " + str(lists[c].index) + " and value " + str(lists[c].value) + "\n")

SHARES = list(lists)
PK = SHARES.pop(0)

m = random.randint(1,q)
print("Message : " + str(m) + "\n")

(R,C) = ENC(PK,m,g,p,q)
print("R : " + str(R) + "\n")
print("C : " + str(C) + "\n")

sublist = random.sample(SHARES,t+1)
print("Selected Shares : \n")
for d in sublist:
    print("Selected Share IDX " + str(d.index) + " with value " + str(d.value) + "\n")

print("\n")
D_LISTE = []
for e in sublist:
    result = DEC(PK,e,R,C,p,q)
    print("D_Share " + str(result.index) + " is " + str(result.value) + "\n")
    D_LISTE.append(result)


restored = RECOVER(PK, D_LISTE, R, C, p, q)
print("restored message " + str(restored) + "\n")

print("Restored equal to start value ? : " + str(restored == m) + "\n")








