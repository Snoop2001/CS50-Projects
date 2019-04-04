#include <stdio.h>
#include <cs50.h>
#include <math.h>

int is_valid(long card, int len);
long get_card(string prompt);
int get_company(long card, int len);

int main(void)
{
    long card = get_card("What is your credit card number?\n");
    long fake = card;
    int length = 1;
    while(fake /= 10)
        length++;

    int valid = is_valid(card, length);  
    if (valid == 1)
    {
        int company = get_company(card, length);
        if (company == 1)
        {
            printf("AMEX\n");
        }
        else if (company == 2)
        {
            printf("MASTERCARD\n");
        }
        else if (company == 3)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else 
    {
        printf("INVALID\n");
    }
    
    // 378282246310005  5555555555554444  23243156789234
}


long get_card(string prompt)
{
    long n;
    do
    {
        n = get_long("%s", prompt); 
    }
    while (n < 1000000000 || n > 9999999999999999);
    return n;
}

int is_valid(long card, int len)
{
    int type;
    long digit_sum = 0;
    long other_sum = 0;
    for (int i = 1; i < len; i+=2)
    {
        long digit = floor(card/(pow(10, i))) - floor(card/(pow(10,(i+1))))*(pow(10,1));
        long product = digit * 2;
        if (product < 10)
        {
            digit_sum+= product;
        }
        else
        {
            long x1 = product - 10;
            digit_sum += (x1 + 1);
        }
    }
    
    for (int i = 0; i < len; i+=2)
    {
        long digit = floor(card/(pow(10, i))) - floor(card/(pow(10,(i+1))))*(pow(10,1));
        other_sum += digit;
    }
    
    long final_sum = other_sum + digit_sum;
    
    if (final_sum%10 == 0)
    {
        type = 1;
    }
    else
    {
        type = 0; 
    }
    
    return type;
}

int get_company(long card, int len)
{
    int type;
    long first_Two = floor(card/(pow(10, (len-2))));
    long first_num = floor(card/(pow(10, (len-1))));
    
    if (first_Two == 34 || first_Two == 37)
    {
        type = 1; 
    }
    else if (first_Two == 51 || first_Two == 52 || first_Two == 53 || first_Two == 54 || first_Two == 55)
    {
        type = 2;
    }
    else if (first_num == 4 && (len == 13 || len == 16))
    {
        type = 3;
    }
    else
    {
        type = 0;
    }
    
    return type;
}

//5673598276138003 4062901840
