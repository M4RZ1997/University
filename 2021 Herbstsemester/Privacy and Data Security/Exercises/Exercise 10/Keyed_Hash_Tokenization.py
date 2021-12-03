import hmac
from Luhn_Algorithm import luhn


def mac_token(key, s):
    if not luhn(s):
        return "Number is not a Luhn-valid number"

    N = len(str(s))
    input_bytes = str(s).encode('ascii')
    key_bytes = str(key).encode('ascii')

    while True:
        input_bytes = hmac.digest(key_bytes, input_bytes, "sha256")

        token = int.from_bytes(input_bytes, "big") % 10**N
        if luhn(token):
            return token


if __name__ == "__main__":
    number = 79927398713
    k = 7366893
    print("Hash-Token of", number, " :", mac_token(k, number))
    number = 15781664523
    k = 154546
    print("Hash-Token of", number, " :", mac_token(k, number))
    number = 64108326501
    k = 123454
    print("Hash-Token of", number, " :", mac_token(k, number))
