#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    //open file card
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
    unsigned char buffer[512];

    //store image title
    char title[8];

    //image index
    int index = 0;

    FILE *img = NULL;

    //read 512 byte of data at a time
    fread(buffer, 512, 1, card);

    //repeat until end of file
    while (!feof(card))
    {
        //is it a start of new jpg?
        if (buffer[0] == 0xff &&
        buffer[1] == 0xd8 &&
        buffer[2] == 0xff &&
        (buffer[3] & 0xf0) == 0xe0)
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
            fwrite(buffer, 512, 1, img);
            int no_read = fread(buffer, 1, 512, card);

            //writes following bytes until new jpeg
            while (!(buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0))
            {
                fwrite(buffer, 1, no_read, img);
                no_read = fread(buffer, 1, 512, card);
            }
        }
        else
        {
            fread(buffer, 512, 1, card);
        }
    }

    //close card file
    fclose(card);

    //success
    return 0;
}
