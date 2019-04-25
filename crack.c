#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <stdlib.h>
#include <string.h>

#define passLength 5
#define charLength 53

int main(int argc, string argv[])
{
    //brian:51.xJagtPnb6s
    //bjbrown:50GApilQSG3E2
    //emc:502sDZxA/ybHs
    //greg:50C6B0oz0HWzo
    //jana:50WUNAFdX/yjA
    //lloyd:50n0AAUD.pL8g
    //malan:50CcfIk1QrPr6
    //natmelo:50JIIyhDORqMU
    //rob:51v3Nh6ZWGHOQ
    //veronica:61v1CDwwP95bY
    //walker:508ny6Rw0aRio
    //zamyla:50cI2vYkF0YU2 = LOL
    
    //ensure correct usage
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    //save old hash
    char *oldHash = argv[1];

    //separate salt and save 
    char salt[3];
    strncpy(salt, oldHash, 2);
    
    //possible char for password
    const char *allChar = "\0abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    //array for new password
    char newPass[6];
    
    for (int fifth = 0; fifth < charLength; fifth++)
    {
        for (int fourth = 0; fourth < charLength; fourth++)
        {
            for (int third = 0; third < charLength; third++)
            {
                for (int second = 0; second < charLength; second++)
                {
                    for (int first = 0; first < charLength; first++)
                    {
                        //this structure will test 1 length passwords, then 2 length, etc...
                        newPass[0] = allChar[first];
                        newPass[1] = allChar[second];
                        newPass[2] = allChar[third];
                        newPass[3] = allChar[fourth];
                        newPass[4] = allChar[fifth];
                        
                        //for every tried password, output the hash 
                        char *attempt = crypt(newPass, salt);
                        
                        //if the tried hash matches the old hash, print the password
                        if (strcmp(attempt, oldHash) == 0)
                        {
                            printf("%s\n", newPass);
                            return 0;
                        }
                    }
                }
            }
        }
    }
    
    //if no password found, print that and return 2 for unsuccessful
    printf("Password could not be found\n");
    return 2;
}
