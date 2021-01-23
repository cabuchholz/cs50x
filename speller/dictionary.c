// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Counter for words added to dictionary
unsigned int nwords = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        //if file is not there/cannot be opened
        return false;
    }

    //define buffer to scan word into
    char word[LENGTH + 1];

    //Read in each word, and stop when EOF

    while (fscanf(file, "%s", word) != EOF)
    {
        //Take each word, and create a new node
        node *entry = malloc(sizeof(node));

        if (entry == NULL)
        {
            return false;
        }
        
        //try iniatlizing the memory 
        memset(entry, 0, sizeof(node));
        
        //Copy the word in dictionary into the node
        strcpy (entry->word, word);

        //Add to the counter of words
        nwords += 1;

        //Hash the word to get index
        unsigned int index = hash(word);

        //Place the pointer address in the Hash Table at index
        //If no entry in that index exists
        if (table[index] == NULL)
        {
            //point to node
            table[index] = entry;
        }
        //if entry in index already exists
        else
        {
            //Set new node to point at original first node
            entry->next = table[index];

            //Set the hash table index pointer to the new node
            table[index] = entry;
        }
    }

    //Close the dictionary file
    fclose(file);

    //Return true as ran successfully
    return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Simple Hash: First letter of word
    int x = ((int)tolower(word[0]) - 97);
    return x;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return nwords;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //Hash the word to get index
    unsigned int index = hash(word);

    //Check to see if any words in that hash code index
    if (table[index] == NULL)
    {
        //not pointer means no words stored in dict for this value
        return false;
    }

    //if there is a pointer at that index in hash table
    else
    {
        //set a cursor equal to the pointer
        node *cursor = table[index];

        //Loop through linked list at Hash Index to see if word is present
        while (cursor != NULL)
        {
            //Compare word to dictionary node regardless of capitalization
            if (strcasecmp(cursor->word, word) == 0)
                {
                    return true;
                }
            //if not same, go to next word in linked list
            else
            {
               cursor = cursor->next;
            }
        }

        //free the cursor
        //free(cursor);

        //if not found in the loop above
        return false;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //Create a counter for words released
    unsigned int count = 0;

    //Iterate through all indexs in the Hash Table
    for (int i = 0; i < N; i++)
    {
       //Check if index has pointer for linked list
       if (table[i] != NULL)
       {
           //set cursor variable equal to first node in linked list
           node *cursor = table[i];

           //Loop through all of the nodes in list, and end when hit NULL
           while (cursor != NULL)
           {
               //Set temp to be cursor
               node *tmp = cursor;

               //Set cursor to point at the next node in linked list
               cursor = cursor->next;

               //free the original node
               free(tmp);

               //Add to freed word counter
               count++;
           }

           //free the cursor
           //free(cursor);
       }
    }

    // Ensure all words were released
    if (count == nwords)
    {
        return true;
    }
    else
    {
        return false;
    }
}

