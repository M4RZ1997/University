from Luhn_Algorithm import luhn


def cycle_walk(cycle, s):
    N = len(str(s))
    input_bytes = str(s).encode('ascii')

    while True:
        input_bytes = cycle(input_bytes)

        token = int.from_bytes(input_bytes, "big") % 10**N
        if luhn(token):
            return token
