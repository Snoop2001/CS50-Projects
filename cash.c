#include <stdio.h>
#include <math.h>
#include <cs50.h>

float get_pos_float(string prompt);

int main(void)
{
    float dollars = get_pos_float("How much are you owed?\n");
    int cents = round(dollars* 100);
    
    int quarters = round(cents / 25);
    cents = cents - quarters*25;
    
    int dimes = round(cents / 10);
    cents = cents - dimes*10;
    
    int nickels = round(cents / 5);
    cents = cents - nickels*5;
    
    int pennies = cents;
    
    int coins = quarters + dimes + nickels + pennies;
    printf("%d\n", coins);
}

float get_pos_float(string prompt)
{
    float n;
    do
    {
        n = get_float("%s", prompt);
    }
    while (n < 0);
    return n;
}
