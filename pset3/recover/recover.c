#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover file\n");
        return 1;
    }
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    int first = 0;
    BYTE buffer[512];
    int i = 0;
    char filename[8];;
    FILE *img;
    int num = fread(buffer, sizeof(BYTE), 512, inptr); // num < 512 if EOF reached
    while (num == 512)
    {
        if (first == 0)// no start of image not found yet
        {
            // ###.jpg starting from 000
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) // start of jpg
            {
                first = 1;
                printf("here1");
                sprintf(filename, "%03i.jpg", i);
                printf("here2");
                img = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), 512, img);
                i++;
                num = fread(buffer, sizeof(BYTE), 512, inptr);
            }
            else
            {
                num = fread(buffer, sizeof(BYTE), 512, inptr);
            }
        }
        else // we have previously found a start of image
        {
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) // new start of jpg
            {
                printf("New image\n");
                fclose(img);
                printf("Closed %s\n", filename);
                sprintf(filename, "%03i.jpg", i);
                img = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), 512, img);
                i++;
                num = fread(buffer, sizeof(BYTE), 512, inptr);
            }
            else
            {
                fwrite(buffer, 512, sizeof(BYTE), img);
                num = fread(buffer, sizeof(BYTE), 512, inptr);
            }
        }
    }
    fclose(img);
    fclose(inptr);
}
