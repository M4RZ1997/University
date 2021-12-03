import hashlib
from Luhn_Algorithm import luhn
from Cycle_Walking import cycle_walk


def hash_token(s):
    try:
        int(s)
    except ValueError:
        return "Input is not a number"

    if not luhn(s):
        return "Number is not a Luhn-valid number"

    return cycle_walk(lambda x: hashlib.sha256(x).digest(), s)


if __name__ == "__main__":
    number = "79927398713"
    print("Hash-Token of", number, " :", hash_token(number))
    number = "15781664523"
    print("Hash-Token of", number, " :", hash_token(number))
    number = "6410H326501"
    print("Hash-Token of", number, " :", hash_token(number))
