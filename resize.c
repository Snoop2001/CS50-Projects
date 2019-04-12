// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: n infile outfile\n");
        return 1;
    }

    // resize factor
    int n = strtol(argv[1], NULL, 10);
    if (n > 100 || n < 0)
    {
        printf("Usage: n infile outfile\n");
        return 1;
    }


    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bfn;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bfn = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bin;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bin = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 4;
    }

    //resize header files
    bin.biHeight = bi.biHeight*n;
    bin.biWidth = bi.biWidth*n;

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outpadding = (4 - (bin.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine new image sizes
    bfn.bfSize = 54 + bin.biWidth * abs(bin.biHeight) * 3 + abs(bin.biHeight) *  outpadding;
    bin.biSizeImage = ((((bin.biWidth * bin.biBitCount) + 31) & ~31) / 8) * abs(bin.biHeight);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfn, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bin, sizeof(BITMAPINFOHEADER), 1, outptr);



    //  resize
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for (int m = 0; m <n; m++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int l = 0; l < n; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            for (int k = 0; k < outpadding; k++)
            {
                fputc(0x00, outptr);
            }

            //allows the program to rewrite each line. Moves cursor back to beginning pixel
            if (m < n - 1)
            {
                fseek(inptr, -bi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
            }

        }
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

    }


    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
