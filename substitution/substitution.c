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
    //Check if each element in the command-line array is a character
    // exclude first string as is program name
    int nonalpha = 0;
    for (int i = 1; i < argc; i++)
    {
        for (int j = 0; j < strlen(argv[i]); j++)
        {
            if (isalpha(argv[i][j]) == 0) //checks if is a alpha, 0 if not
            {
                nonalpha = nonalpha + 1;
            }
        }
    }
    
        //Check if # of command line argument is correct
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    //Ensures all are alphabetical
    else if (nonalpha > 0)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    //Ensure is 26 characters long
    else if (strlen(argv[1]) != 26)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    //Ensure is 26 characters long
    else if (strlen(argv[1]) != 26)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    else 
    {
    
    //Check if each element in the command-line is unique
   int repeat = 0;
   int len = strlen(argv[1]);
   string cipher = argv[1];
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            if (tolower(cipher[i]) == tolower(cipher[j]))
            {
                repeat = repeat + 1;
            }
        }
    }


    
     //Ensure each alpha character only happens once 
    if (repeat > 26)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    else
    {
        //Prompt for the input text
        string text = get_string("plaintext: ");
        printf("ciphertext: ");
        //Iterate through and encrypt the text 
        for (int i = 0, lentext = strlen(text); i < lentext; i++)
        {
            if (isalpha(text[i]) == 0)
            {
                printf("%c", text[i]);
            }
            else if (isupper(text[i]) > 0) //checks if is upper
            {
                printf("%c", toupper(argv[1][(int) text[i] - 65]));
            }
            else 
            {
               printf("%c", tolower(argv[1][(int) text[i] - 97]));
            }
        }
        printf("\n"); //provide newline for spacing
        return 0; 
    }
    }
}

