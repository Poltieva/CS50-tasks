#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    int output = 0;
    float change = get_float("Change: ");
    while (change < 0)
    {
        change = get_float("Change owed: ");
    }
    int coins = round(change * 100);
    while (coins >= 25)
    {
        output++;
        coins = coins - 25;
    }
    while (coins >= 10)
    {
        output++;
        coins = coins - 10;
    }
    while (coins >= 5)
    {
        output++;
        coins = coins - 5;
    }
    while (coins > 0)
    {
        output++;
        coins--;
    }
    printf("%i\n", output);
}