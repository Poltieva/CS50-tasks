# asking for the height of the pyramid
h = int(input("Height: "))
while h < 1 or h > 8:
    h = int(input("Height: "))
# building the pyramid
for i in range(1, h+1):
    print(" " * (h - i) + "#" * i + "  " + "#" * i)