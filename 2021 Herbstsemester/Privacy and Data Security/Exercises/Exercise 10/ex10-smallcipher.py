# Encryption on an arbitrary (small) domain, using a Feistel network:
# John Black, Phillip Rogaway:
# Ciphers with Arbitrary Finite Domains. CT-RSA 2002: 114-130
# https://doi.org/10.1007/3-540-45760-7_9
#
# This implementation is FE2 as described in Fig. 3 of:
# Mihir Bellare, Thomas Ristenpart, Phillip Rogaway, Till Stegers:
# Format-Preserving Encryption. Selected Areas in Cryptography 2009: 295-312
# https://doi.org/10.1007/978-3-642-05445-7_19
#
# Encrypts `m` in `[0, k-1]` for `k = a*b` with `r` rounds, using key `key`
# and tweak `tweak`. Invokes HMAC-`hashalg` for the PRF (one could also use
# AES for this, but it requires special libraries in Python).  Bellare et
# al prove that `r >= 3` is secure against nonadaptive chosen-plaintext
# attacks.
#
import hmac
import sys


def smallcipher_encrypt(key, tweak, hashalg, m, a, b, r):
	L = m // b
	R = m % b
	for i in range(1, r + 1):
		if (i & 0x01):
			s = a
		else:
			s = b
		tmp = R
		rstr = (str(a) + '|' + str(b) + '|' + str(tweak) + '|' + str(i)
			+ '|' + str(R)).encode('ascii')
		f = int.from_bytes(hmac.digest(key, rstr, hashalg), 
				   byteorder=sys.byteorder)
		R = (L + f) % s
		L = tmp
	return s * L + R

