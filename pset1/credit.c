#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

bool valid(string card_num);
void reverse(char s[]);
void itoa(int n, char s[]);

int main(void)
{
    string card = get_string("Number: ");

    while (!valid(card))
    {
        card = get_string("Number: ");
    }

    int sum = 0;
    bool even_index;
    int len = strlen(card);
    if (len < 13)
    {
        printf("INVALID\n");
        return 0;
    }
    for (int n = len - 1; n >= 0; n--)
    {
        char a = card[n];
        if (((len % 2 == 0) && (n % 2 == 0)) || ((len % 2 == 1) && (n % 2 == 1)))
        {
            int y = atoi(&a) * 2;
            if (y < 10)
            {
                char num[1];
                itoa(y, num);
                for (int t = 0; t < strlen(num); t++)
                {
                    char e = num[t];
                    sum += atoi(&e);
                }
            }
            else
            {
                char num[2];
                itoa(y, num);
                for (int t = 0; t < strlen(num); t++)
                {
                    char e = num[t];
                    sum += atoi(&e);
                }
            }

        }
        else
        {
            sum += atoi(&a);
        }
    }
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }
    else
    {
        if ( (strncmp(&card[0], "3", 1) == 0) && ( (strncmp(&card[1], "4", 1) == 0) || (strncmp(&card[1], "7", 1) == 0) ))
        {
            printf("AMEX\n");
            return 0;
        }
        if (strncmp(&card[0], "4", 1) == 0)
        {
            printf("VISA\n");
            return 0;
        }
        if ( (strncmp(&card[0], "5", 1) == 0) && ( (strncmp(&card[1], "1", 1) == 0) || (strncmp(&card[1], "2", 1) == 0) ||
        (strncmp(&card[1], "3", 1) == 0) || (strncmp(&card[1], "4", 1) == 0) || (strncmp(&card[1], "5", 1) == 0) ))
        {
            printf("MASTERCARD\n");
            return 0;
        }
        else
        {
            printf("INVALID\n");
            return 0;
        }
    }
}

bool valid(string card_num)
{
    for (int i = 0; i < strlen(card_num); i++)
        {
            if (!isdigit(card_num[i]))
            {
                return false;
            }
        }
    return true;
}
 /* reverse:  переворачиваем строку s на месте */
 void reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
/* itoa:  конвертируем n в символы в s */
 void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* записываем знак */
         n = -n;          /* делаем n положительным числом */
     i = 0;
     do {       /* генерируем цифры в обратном порядке */
         s[i++] = n % 10 + '0';   /* берем следующую цифру */
     } while ((n /= 10) > 0);     /* удаляем */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }
