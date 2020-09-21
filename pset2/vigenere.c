#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int shift(char c);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./vigenere key\n");
        return 1;
    }
    int num = 0;
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]))
        {
            num++;
        }
    }
    if (num != strlen(argv[1]))
    {
        printf("Usage: ./vigenere key\n");
        return 1;
    }
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");
    for (int c = 0, k = 0; c < strlen(plaintext); c++)
    {
        if (isalpha(plaintext[c]))
        {
            int key = shift(argv[1][k]);
            if (isupper(plaintext[c]))
            {
                printf("%c", ((shift(plaintext[c]) + key) % 26) + 65);
            }
            if (islower(plaintext[c]))
            {
                printf("%c", ((shift(plaintext[c]) + key) % 26) + 65);
            }
            if (k == strlen(argv[1]) - 1)
            {
                k = 0;
            }
            else
            {
                k++;
            }
        }
        else
            {
                printf("%c", plaintext[c]);
            }
    }
        printf("\n");
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
