from cs50 import get_string
from sys import argv, exit
import re


def censor(text, banned_words):
    # for censored phrase
    final = []
    
    # check if word needs to be censored
    for word in text:
        if (word.lower()).strip(",.!?") in banned_words:
            censored = re.sub('[a-zA-Z]', '*', word)
            final.append(censored)
        else:
            final.append(word)
    
    #return censored phrase
    return ' '.join(final)
    
def main():
    # ensure correct usage
    if len(argv) != 2:
        print("Usage: python bleep.py dictionary")
        exit(1)
    
    # open dictionary of bad words
    name = argv[1] 
    with open(name, 'r', encoding="UTF-8") as f:
        lines = [line.rstrip() for line in f]
        
    # get user input to censor and split into words
    to_censor = input("What message would you like to censor? ")
    to_censor = to_censor.split()
    
    #censor and obtain new phrase
    censored = censor(to_censor, lines)
    print(censored)
    

if __name__ == "__main__":
    main()
