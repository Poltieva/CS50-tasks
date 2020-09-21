from sys import argv, exit

def main():
    if len(argv) != 2:
        print("Usage: python bleep.py dictionary")
        exit(1)

    dict = argv[1]
    with open(dict, "r") as file:
        banned_words = set(word.rstrip("\n") for word in file.readlines())
    
    message = input("What message would you like to censor?\n").split()
    message1 = []
    for w in message:
        if w.lower() in banned_words:
            w = "*" * len(w)
        message1.append(w)
    print(" ".join(message1))
if __name__ == "__main__":
    main()
