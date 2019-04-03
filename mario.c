#include <cs50.h>
#include <stdio.h>

void build(int n);
int get_positive_int(string prompt);

int main(void)
{
    //get size of building
    int size = get_positive_int("How tall do your want the tower?\n");
    
    build(size); 
}

void build(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%*s", n-i, "");
        printf("%.*s", i+1, "#################################################");
        printf("%*s", 2, " ");
        printf("%.*s\n", i+1, "################################################");
    }
}


int get_positive_int(string prompt)
{
    int n;
    do
    {
        n = get_int("%s", prompt);
    }
    while (n < 1);
    return n;
}
