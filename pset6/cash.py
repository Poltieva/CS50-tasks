
while True:
    try:
        change = float(input("Change owed: "))
        if 0.01 <= change:
            break
    except ValueError:
        continue

coins = round(change * 100)
twenty_five_cent = coins // 25
ten_cent = (coins % 25) // 10
five_cent = ((coins % 25) % 10) // 5
pennies = (((coins % 25) % 10) % 5) / 1
print(int(twenty_five_cent + ten_cent + five_cent + pennies))