from cs50 import get_string
from sys import argv, exit


def main():
    # ensure correct usage
    if len(argv) != 2:
        print("Usage: python bleep.py dictionary")
        exit(1)
    
    # open dictionary of bad words
    name = argv[1] 
    text = open(name, "r", encoding="latin_1")
    if not text:
        print("Could not open {}.".format(name))
        unload()
        exit(1)
        
    # store banned words in list
    bannedList = []
    for ban in text:
        ban = ban.replace('\n', '')
        bannedList.append(ban)
        
    # get user input to censor and split into words
    toCensor = input("What message would you like to censor? ")
    toCensor = toCensor.split()
    
    # for censored phrase
    final = []
    
    # check if word needs to be censored
    for word in toCensor:
        if word.lower() in bannedList:
            censored = word.replace(word, "*"*len(word))
            final.append(censored)
        else:
            final.append(word)
    
    print(' '.join(final))
            

if __name__ == "__main__":
    main()
