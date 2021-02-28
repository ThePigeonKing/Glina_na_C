s = input("Enter your string: ")

key = "ponpon"

for i, c in enumerate(s):
    if ord(c) > 255:
        print("ASCII only")
        exit()

    print(ord(c) ^ ord(key[i%len(key)]), end=" ")

print("\n")
