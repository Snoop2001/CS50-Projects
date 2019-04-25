#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

int main(int argc, string argv[])
{
    //brian:51.xJagtPnb6s = TF
    //bjbrown:50GApilQSG3E2 = UPenn
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

    //separate salt and hash
    char salt[3];
    char *key = &argv[1][2];
    char *hash = argv[1];
    strncpy(salt, hash, 2);

    
    
    
    
    
    

}
