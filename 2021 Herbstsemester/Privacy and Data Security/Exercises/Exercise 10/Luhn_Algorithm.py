"""
Implementation of the Luhn-Algorithm assuming that the last digit is the checksum
(it is also possible that it is the first digit)
"""


def luhn(s):
    last_digit = s % 10
    s_without_last = str(s)[:-1]
    sum = 0
    multiplier = 1
    for number in s_without_last:
        temp = int(number) * multiplier
        while temp > 10:
            temp_string = str(temp)
            temp = 0
            for t in temp_string:
                temp += int(t)
        sum += temp

        if multiplier == 1:
            multiplier = 2
        else:
            multiplier = 1

    checksum = 10 - sum % 10

    return checksum == last_digit


if __name__ == "__main__":
    print("Is a Luhn-correct checksum:", luhn(989373674))
    print("Is a Luhn-correct checksum:", luhn(79927398713))
