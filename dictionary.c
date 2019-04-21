// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

//count words as I go
int num_words = 0;

//help indicate loading
bool loaded = false;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        //count number of words
        num_words++;

        // make space for new node
        node *new_node;
        new_node = malloc(sizeof(node));

        //ensure we have the memory
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        //if we do, fill the node with the word
        else
        {
            strcpy(new_node->word, word);
            new_node->next = NULL;
            int bucket = hash(word);

            if(hashtable[bucket] == NULL)
            {
                hashtable[bucket] = new_node;
            }
            else
            {
                new_node->next = hashtable[bucket];
                hashtable[bucket] = new_node;
            }
        }
    }

    // Close dictionary
    fclose(file);

    printf("%ld", sizeof(hashtable));

    // Indicate success
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (loaded)
    {
        return num_words;
    }
    return 0;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{

    //make a pointer to traverse
    node *ptr;

    //get bucket that the word is in
    int bucket = hash(word);

    //point ptr to the first node in that bucket's linked list
    ptr = hashtable[bucket];

    //if the dictionary is loaded, begin traversing through linked list
    if (loaded)
    {
        //loop until the end of linked list
        while (ptr != NULL)
        {
            //if the next node has the word, return true
            if (strcasecmp(ptr->word, word) == 0)
            {
                return true;

            //if it doesn't, point to the next node
            }
            else
            {
                ptr = ptr->next;
            }
        }
        //if you get through while loop without finding word, return false
        return false;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *ptr, *temp;
    for (int i = 0; i < N; i++)
    {
        ptr = hashtable[i];
        while (ptr != NULL)
        {
            temp = ptr;
            ptr = ptr->next;
            free(temp);
        }
    }
    return true;
}
