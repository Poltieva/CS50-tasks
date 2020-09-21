#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember info
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];
    if (n < 1 || n > 100)
    {
        printf("n must be a positive integer less than 101\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    BITMAPFILEHEADER bf2 = bf;

    // BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    BITMAPINFOHEADER bi2 = bi;
    int in_width = bi.biWidth;
    int in_height = bi.biHeight;
    bi2.biWidth = in_width * n;
    bi2.biHeight = in_height * n;

    int padding = (4 - (in_width * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding2 = (4 - (bi2.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi2.biSizeImage = (bi2.biWidth * sizeof(RGBTRIPLE) + padding2) * abs(bi2.biHeight);
    bf2.bfSize = bi2.biSizeImage + 54;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf2, sizeof(BITMAPFILEHEADER), 1, outptr);


    // write outfile's BITMAPINFOHEADER
    fwrite(&bi2, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    int abs_in_height = abs(in_height);
    for (int i = 0, biHeight = abs_in_height; i < biHeight; i++)
    {
        for (int v = 0; v < n; v++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < in_width; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                for (int h = 0; h < n; h++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding2; k++)
            {
                fputc(0x00, outptr);
            }
            fseek(inptr, -((in_width * sizeof(RGBTRIPLE)) + padding), SEEK_CUR);
        }
        fseek(inptr, (in_width * sizeof(RGBTRIPLE)) + padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
