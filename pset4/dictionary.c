// Implements a dictionary's functionality

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>

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
node* hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char word)
{
    return tolower(word) - 'a';
}

//number of words in dicitonary
int size_of_dict = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int y = 0; y < N; y++)
    {
        node * head = NULL;
        hashtable[y] = head;
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
        node* new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        strcpy(new_node->word, word);

        //adding to hash table
        int index = hash(new_node->word[0]);
        new_node->next = hashtable[index];
        hashtable[index] = new_node;
        size_of_dict++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return size_of_dict;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int index = hash(word[0]);
    if (hashtable[index] == NULL)
    {
        return false;
    }
    node* current = hashtable[index];
    while (current != NULL) {
        if (strcasecmp(word, current->word) == 0)
        {
            return true;
        }
        current = current->next;
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int index = 0; index < N; index++)
    {
        node * current = hashtable[index];
        while (current != NULL)
        {
            node * tmp = current;
            current = current->next;
            free(tmp);
        }
        hashtable[index] = NULL;
    }
    return true;
}

