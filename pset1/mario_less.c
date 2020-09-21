#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // asking for the height of the pyramid
    int height = get_int("Height: ");
    while (height < 1 || height > 8)
    {
        height = get_int("Height: ");
    }
    // building the pyramid
    int i = 1;
    while (i <= height)
    {
        int t = 0;
        int n = height - i;
        while (n > 0)
        {
            printf(" ");
            n--;
        }
        while (t < i)
        {
            printf("#");
            t++;
        }
        printf("\n");
        i++;
    }
}
