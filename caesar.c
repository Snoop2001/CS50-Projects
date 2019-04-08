#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    //check for valid key and save
    int key = 0;
    int valid = 0;
    if (argc == 2)
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            int c = (int) argv[1][i];
            if (c > 47 && c < 58)
            {
                valid = 1;
            }
            else
            {
                valid = 0;
                return 0;
                break;
            }
        }
    }
    else
    {
        printf("Usage: ./caesar key\n");  
        return 0;
    }
    
    //create key
    if (valid == 1)
    {
        int key_l = strtol(argv[1], NULL, 10);
        key = key_l % 26;
    }

    //ask for plaintext: and will accept anything, evening nothing
    string plaintext = "";
    if (valid == 1)
    {
        plaintext = get_string("plaintext:  ");       
    }
    else
    {
        printf("Usage: ./caesar key\n");
    }

    //Iterate over char in plaintext and prints ciphertext
    char new_char;
    int new_code;
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        // print each character of plaintext 
        int c = (int) plaintext[i];
        if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
        {
        
            //Capitals
            if (c >= 65 && c <=90)
            {
                new_code = c + key; 
                if (new_code > 90)
                {
                    new_code = new_code - 26;
                    new_char = (char) new_code;
                    printf("%c", new_char);
                }
                else
                {
                    new_char = (char) new_code;  
                    printf("%c", new_char);
                }

            }
            
            //Lowercases
            else if (c >= 97 && c <= 122)
            {
                new_code = c + key; 
                if (new_code > 122)
                {
                    new_code = new_code - 26;
                    new_char = (char) new_code;
                    printf("%c", new_char);
                }
                else
                {
                    new_char = (char) new_code;  
                    printf("%c", new_char);
                }

            }   
        }
          
        else
        {
            new_char = (char) c;
            printf("%c", new_char);
        }       
    }
    printf("\n");
}
    


