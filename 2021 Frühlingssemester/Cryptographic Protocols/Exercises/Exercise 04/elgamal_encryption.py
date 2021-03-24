from Cryptodome.Random import random
from Cryptodome.Util import number
import time

################################################################################################
##                            Code from Diffie-Hellman-Example                                ##
################################################################################################
def power(a, x, p):
    res = 1
    a = a % p
    if (a == 0):
        return 0
    while (x > 0):
        if ((x & 1L) == 0):
            a = (a * a) % p
            x = x >> 1
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
################################################################################################
##                                           End                                              ##
################################################################################################

################################################################################################
##                          (additive homomorphic) ElGamal Code                               ##
################################################################################################
def enc(pk, m, g, q, p):
    r = random.randint(2, q-1)
    R = power(g, r, p)
    C = (m * power(pk, r, p)) % p
    return (R, C)

def dec(sk, R, C, p):
    r1 = power(R, sk, p) % p
    r2 = number.inverse(r1, p)
    m = (C * r2) % p
    return m

def enc_ah(pk, a, g, q, p):
    ga = power(g, a, p) % p
    return enc(pk, ga, g, q, p)

def dec_ah(sk, R, C ,p, max):
    h = dec(sk, R, C, p)
    # brute forcing
    for x in range(max + 1):
        gi = power(g, x, p) % p
        if (gi == h):
            return x
    return "ERROR"
################################################################################################
##                                           End                                              ##
################################################################################################

# Generating g, q and p
(g, q, p) = Parameters(160, 1024)
#(g, q, p) = Parameters(256, 2048)

# Key-Generation (ElGamal style)
sk = random.randint(2, q-1)
pk = power(g, sk, p)


################################################################################################
##                         Testing decryption of encryption numbers                           ##
################################################################################################
maxList = [100, 256, 1000, 2560, 10000, 25600, 100000, 256000, 1000000]

for i in maxList:
    print("----------------------------------------------------------------------")
    print("Value of Max:" + str(i))
    x = 0
    while (x < 5):
        x = x+1
        a = random.randint(1, i)
        print("number a is " + str(a) + "\n")
        (R, C) = enc_ah(pk, a, g, q, p)
        print("is C == a ? " + str((a == C)) + "\n")
        print("number C is " + str(C) + "\n")
        t1 = time.time()
        a1 = dec_ah(sk, R, C, p, i)
        t2 = time.time()
        print("decryption took " + str(t2 - t1) + " seconds \n")
        print("number a1 is " + str(a1) + "\n")
        print("is a1 == a ? " + str((a == a1)) + "\n")
    print("----------------------------------------------------------------------")
################################################################################################
##                                           End                                              ##
################################################################################################

################################################################################################
##                                Testing additive behaviour                                  ##
################################################################################################
# we are taking half of MAX because otherwise there might occur an Error when decrypting c2
# MAX = 10000
# a = random.randint(1, MAX/2)
# print("number a is " + str(a) + "\n")
# b = random.randint(1, MAX/2)
# print("number b is " + str(b) + "\n")
# c = (a + b) % p
# print("number c is " + str(c) + "\n")
# (R1, C1) = encryptAH(pk, a, g, q, p)
# (R2, C2) = encryptAH(pk, b, g, q, p)
# R3 = (R1 * R2) % p
# C3 = (C1 * C2) % p
# c2 = decryptAH(sk, R3, C3, p, MAX)
# print("number c2 is " + str(c2) + "\n")
# print("number c == c2 " + str(c == c2) + "\n")
################################################################################################
##                                           End                                              ##
################################################################################################