#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int shift(char c);

int main(int argc, string argv[])
{
    //check for valid key and save
    int key;
    int new_code;
    int valid = 0;
    if (argc == 2)
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            int c = (int) tolower(argv[1][i]);
            if (c >= 97 && c <= 122)
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
    
    if (valid == 1)
    {
        string plaintext = get_string("plaintext: ");
        printf("ciphertext: ");
        for (int j = 0, l = strlen(plaintext); j < l; j++)
        {
            key = shift(tolower(argv[1][j%strlen(argv[1])])); 
            int y = (int) plaintext[j];
            if ((y >= 65 && y <= 90) || (y >= 97 && y <= 122))
            {
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
                printf("%c", (char) y);
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

int shift(char c)
{
    int key = (int) c - 97;
    return key;
}
