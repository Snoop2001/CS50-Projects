#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int shift(char c);

int main(int argc, string argv[])
{
    //check for valid key and save
    int valid = 0;
    if (argc == 2)
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (isalpha(argv[1][i]))
            {
                valid = 1; 
            }
            else
            {
                printf("Usage: ./vigenere keyword\n");
                return 1;
            }
        }
    }
    else
    {
        printf("Usage: ./vigenere keyword\n");  
        return 1;
    }
    
    //begin encryption
    int key;
    int index = -1;
    int new_code;
    if (valid == 1)
    {
        string plaintext = get_string("plaintext: "); 
        printf("ciphertext: ");
        for (int j = 0, l = strlen(plaintext); j < l; j++)
        { 
            if (isalpha(plaintext[j]))
            {
                index++;
                key = shift(tolower(argv[1][index % strlen(argv[1])]));
                new_code = (int) plaintext[j] + key;  
                if ((int) tolower(plaintext[j]) + key > 122)
                {
                    new_code = new_code - 26;
                    printf("%c", (char) new_code); 
                }
                else 
                {
                    printf("%c", (char) new_code);
                }
            }
            else
            {
                printf("%c", (char) plaintext[j]);
            }
        }
        printf("\n");
    }
    else
    {
        printf("Usage: ./vigenere keyword\n");  
        return 1;
    }
}

//create key
int shift(char c)
{
    int key = (int) c - 97;
    return key;
}
