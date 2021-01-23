//Include prototypes
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

//Main function with Command Line Arguements
int main(int argc, string argv[])
{
    //Check if each element in the command-line array is a decimal digit
    // exclude first string as is program name
    int nondigit = 0;
    for (int i = 1; i < argc; i++)
    {
        for (int j = 0; j < strlen(argv[i]); j++)
        {
            if (isdigit(argv[i][j]) == 0) //checks if is a digit, 0 if not
            {
                nondigit = nondigit + 1; 
            }
        }
    }
    
    //Check if # of command line argument is correct
    if (argc != 2)
    {
        //printf("incorrect number of command-line argument(s)\n");
        //return 1;
        printf("Usage: ./caesar key\n");
        return 1;
    }
    
    //print error based on if have nondigits
    else if (nondigit > 0)
    {
        printf("Usage: ./caesar key\n");
        return 1; 
    }
    
    //If no problems with input, prompt user for a key, and offset
    else
    {
        //prompt for key 
        string text = get_string("plaintext: ");
        int lentext = strlen(text);
        //Print the originally entered text offset in command line
        printf("ciphertext: ");
        for (int i = 0; i < lentext; i++)
        {
            if (isalpha(text[i]) == 0)
            {
                printf("%c", text[i]);
            }
            else if (isupper(text[i]) > 0) //checks if is upper
            {
               if (text[i] + atoi(argv[1])%26 > 90)
               {
                   printf("%c", text[i] + atoi(argv[1])%26 - 26);
               }
               else
               {
                   printf("%c", text[i] + atoi(argv[1])%26);
               }
            }
            else 
             {
               if (text[i] + atoi(argv[1])%26 > 122)
               {
                   printf("%c", text[i] + atoi(argv[1])%26 - 26);
               }
               else
               {
                   printf("%c", text[i] + atoi(argv[1])%26);
               }
            }
        }
        printf("\n"); //provide newline for spacing
        return 0; 
    }
}

