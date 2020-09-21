import sys

if len(sys.argv) != 2:
    print("Usage: python caesar.py k")
    sys.exit(1)

key = sys.argv[1]
if not key.isdigit():
    print("Usage: python caesar.py k")
    sys.exit(1)

key = int(key)
text = input("plaintext: ")
print("ciphertext: ", end="")
for c in text:
    if c.isalpha():
        if c.isupper():
            c = chr((((ord(c) - 65) + key) % 26) + 65)
        else:
            c = chr((((ord(c) - 97) + key) % 26) + 97)
    print(c, end="")

print()