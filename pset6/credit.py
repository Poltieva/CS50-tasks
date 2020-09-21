while True:
    num = input("Number: ")
    if num.isdigit():
        break

products = (str(int(el) * 2) for el in num[-2::-2])

prod_sum = 0
for el in products:
    for digit in el:
        prod_sum += int(digit)

end_sum = prod_sum + sum([int(el) for el in num[-1::-2]])
if end_sum % 10 == 0:
    if num.startswith("34") or num.startswith("37"):
        print("AMEX")
    elif num[0] == "5" and num[1] in "12345":
        print("MASTERCARD")
    elif num[0] == "4":
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")