#include <stdio.h>
#include <stdlib.h>

#define BLOCK 512

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    // remember filenames
    char *cardName = argv[1];

    // open card file
    FILE *card = fopen(cardName, "r");
    if (card == NULL)
    {
        printf("Could not open %s.\n", cardName);
        return 2;
    }

    //buffer space
    unsigned char buffer[BLOCK];

    //store image title
    char title[8];

    //image index
    int index = 0;

    //NULL image placeholder
    FILE *img = NULL;

    //read 512 bytes of data at a time
    fread(buffer, BLOCK, 1, card);

    //repeat until end of file
    while (!feof(card))
    {
        //is it a start of new jpg?
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //check if already found
            if (img != NULL)
            {
                fclose(img);
            }

            //if new, writes signature block
            sprintf(title, "%03d.jpg", index);
            index++;
            img = fopen(title, "w");
            fwrite(buffer, BLOCK, 1, img);
            int no_read = fread(buffer, 1, BLOCK, card);

            //writes following bytes until new jpeg
            while (!(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0))
            {
                fwrite(buffer, 1, no_read, img);
                no_read = fread(buffer, 1, BLOCK, card);
                if (no_read == 0)
                {
                    break;
                }
            }
        }
        else
        {
            fread(buffer, BLOCK, 1, card);
        }
    }

    //close card file
    fclose(card);

    //success
    return 0;
}
