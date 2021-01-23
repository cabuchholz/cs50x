//install dependences
#include <stdio.h>
#include <cs50.h>
#include <math.h>

//main function
//Ask for the how much change is owned
int main(void)
{
    float change;
    do
    {
      change = get_float("change owed: ");  
    }
    while(change < 0);
     //convert to integer to avoid floating point precision issue
    int cents = round(change*100); 
    //Divide by 25 cents first, get number and remainder
    int remq = cents % 25; //remainder when use quarters
    int quarters = (cents-remq)/25; //number of quarters
    int remd = remq % 10; //remainder when dimes 
    int dimes = (remq-remd)/10; //numeber of dimes
    int remn = remd % 5; //remainder when nickels
    int nickels = (remd-remn)/5; //number of nickels
    int pennies = remn; //number of pennies based on remainder
    int numcoins = quarters + dimes + nickels + pennies;
    printf("%i\n", numcoins);
}
