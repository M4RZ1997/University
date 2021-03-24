# Parameter generation and implementation of Diffie-Hellman key exchange

# The following code assumes that PyCryptodome has been installed using
#   pip install pycryptodomex
# Documentation for PyCryptodome
#   https://pycryptodome.readthedocs.io/en/latest/

from Cryptodome.Random import random
from Cryptodome.Util import number


# Modular exponentiation using square-and-multiply method
#
# a: base
# x: exponent
# p: modulus
# returns (a^x) mod p computed using O(log p) steps
#
def power(a, x, p):
    res = 1
    a = a % p
    if (a == 0):
        return 0
    while (x > 0):
        # Square if exponent is even
        if ((x & 1) == 0):
            a = (a * a) % p
            x = x >> 1
        # Multiply if exponent is odd
        else:
            res = (res * a) % p
            x = x - 1
    return res


# Generation of a random element in the subgroup

# q: prime
# p: prime such that q divides p - 1
# returns a random element in the subgroup of order q modulo p
#
def randomsubgroup(q, p):
    while True:
        h = random.randint(2, p-1)
        g = power(h, (p-1)/q, p)
        if (g != 1):
            break
    return g


# Generation of system parameters
#
# qbits: bit length of q, the subgroup of prime order q
# pbits: bit length of p, the modulus
# returns (g, q, p), where g is a generator of the subgroup of order q mod p
#
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



# Diffie-Hellman key agreement demo
#

(g, q, p) = Parameters(160, 1024)     # For testing only! INSECURE!
# (g, q, p) = Parameters(256, 2024)     # For use in practice

# Alice
a = random.randint(2, q-1)
A = power(g, a, p)
# Alice sends A to Bob

# Bob
b = random.randint(2, q-1)
B = power(g, b, p)
# Bob sends B to Alice

# Alice computes the shared key
power(B, a, p)

# Bob computes the shared key
power(A, b, p)

print("Hello World")

