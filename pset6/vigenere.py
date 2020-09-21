import sys

def shift(c):
    if c.isupper():
        return ord(c) - 65
    else:
        return ord(c) - 97

def main():
    if len(sys.argv) != 2:
        print("Usage: python caesar.py k")
        sys.exit(1)

    key = sys.argv[1]
    if not key.isalpha():
        print("Usage: python caesar.py k")
        sys.exit(1)
    
    text = input("plaintext: ")
    len_key = len(key)
    k = 0
    ciphertext = ""
    for c in text:
        if c.isalpha():
            s = shift(key[k])
            if c.isupper():
                ciphertext += chr((((ord(c) - 65) + s) % 26) + 65)
            else:
                ciphertext += chr((((ord(c) - 97) + s) % 26) + 97)
            if k == len_key - 1:
                k = 0
            else:
                k += 1
        else:
            ciphertext += c
    print("ciphertext: ", ciphertext)

if __name__ == "__main__":
    main()
