#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>
#include <stdlib.h>


int shift(char c);
int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    int num = 0;
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isdigit(argv[1][i]))
        {
            num++;
        }
    }
    if (num != strlen(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        int n = atoi(argv[1]);
        string plaintext = get_string("plaintext: ");
        printf("ciphertext: ");
        for (int c = 0; c < strlen(plaintext); c++)
        {
            if (isalpha(plaintext[c]))
            {
                if (isupper(plaintext[c]))
                {
                    printf("%c", ((shift(plaintext[c]) + n) % 26) + 65);
                }
                if (islower(plaintext[c]))
                {
                    printf("%c", ((shift(plaintext[c]) + n) % 26) + 97);
                }
            }
            else
            {
                printf("%c", plaintext[c]);
            }
        }
        printf("\n");
    }
}

int shift(char c)
{
   if (isupper(c))
   {
       return c - 'A';
   }
    else
    {
        return c - 'a';
    }
}
