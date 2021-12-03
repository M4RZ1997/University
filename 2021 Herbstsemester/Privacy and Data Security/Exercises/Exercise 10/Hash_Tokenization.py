import hashlib
from Luhn_Algorithm import luhn


def hash_token(s):
    if not luhn(s):
        return "Number is not a Luhn-valid number"

    N = len(str(s))
    input_bytes = str(s).encode('ascii')

    while True:
        input_bytes = hashlib.sha256(input_bytes).digest()

        token = int.from_bytes(input_bytes, "big") % 10**N
        if luhn(token):
            return token


if __name__ == "__main__":
    number = 79927398713
    print("Hash-Token of", number, " :", hash_token(number))
    number = 15781664523
    print("Hash-Token of", number, " :", hash_token(number))
    number = 64108326501
    print("Hash-Token of", number, " :", hash_token(number))
