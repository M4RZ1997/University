import hmac
from Luhn_Algorithm import luhn
from Cycle_Walking import cycle_walk


def mac_token(key, s):
    try:
        int(s)
    except ValueError:
        return "Input is not a number"

    if not luhn(s):
        return "Number is not a Luhn-valid number"

    key_bytes = key.encode('ascii')

    return cycle_walk(lambda x: hmac.digest(key_bytes, x, "sha256"), s)


if __name__ == "__main__":
    number = "79927398713"
    k = "7366893"
    print("Hash-Token of", number, " :", mac_token(k, number))
    number = "15781664523"
    k = "154546"
    print("Hash-Token of", number, " :", mac_token(k, number))
    number = "64108326501"
    k = "123H54T"
    print("Hash-Token of", number, " :", mac_token(k, number))
    number = "15H564G23"
    k = "123H54T"
    print("Hash-Token of", number, " :", mac_token(k, number))
