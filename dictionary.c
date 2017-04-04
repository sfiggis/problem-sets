/**
 * Implements a dictionary's functionality.
*/

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

//create global word counter
int word_count = 0;

// create nodes for hashtable
    typedef struct node
    {
        char word[LENGTH + 1];
        struct node* next;
    } node;
    
    node* hashtable[HASHTABLE_SIZE];
    
//create hashtable
//Hash function (credit to Deplicity, originally found:
//https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/
int hash_it(char* needs_hashing)
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(needs_hashing); i<n; i++)
        hash = (hash << 2) ^ needs_hashing[i];
    return hash % HASHTABLE_SIZE;
}
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    //store length of word including null
    int word_length = (strlen(word) + 1);
    //store lowercase of word
    char lowercase[word_length];
    //iterate through word until reach word length - 1 (null terminator)
    for (int i = 0; i < (word_length); i++)
    {
        lowercase[i] = tolower(word[i]);
    }
    lowercase[word_length] = '\0';
    //use hash function to find bucket/index
    int index = hash_it(lowercase);
    //check if something is in bucket/index
    if (hashtable[index] == NULL)
        return false;
    //traverse through linked list to search for word
    //create cursor, pointed at list's 'head' to traverse through linked list
    node* cursor = hashtable[index];
    //while cursor is not NULL
    while (cursor != NULL)
    //compare word with value at cursor
    {
        if (strcmp(cursor -> word, lowercase) == 0)
            return true;
        else
            cursor = cursor -> next;
    }
    //if reached the end, and not there, return false
    return false;
    //if found more often, move to front of list
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    //open dictionary in read mode, checking it exists.
    FILE* dict = fopen(dictionary, "r");
    if (dict == NULL)
        return false;

    // read file size to determine hashtable size
    
    //create array for new word
    char new_word[LENGTH+1];
    
    // loop through each string in file.
    while (fscanf(dict, "%s\n", new_word) != EOF)
    {  
        //increase wordcount
        word_count ++;
        
        // allocate memory for each new word
        node* new_node = malloc(sizeof(node));
        
        // Add each new word into a new node
        strcpy(new_node -> word, new_word);
        
        //Hash word to get index
        int index = hash_it(new_node -> word);
        
        //If corresponding bucket empty, create linked list
        if (hashtable[index] == NULL)
        {
            hashtable[index] = new_node;
            new_node -> next = NULL;
        }
        //If corresponding bucket greater than 0, append to linked list
        else
        {
            new_node -> next = hashtable[index];
            hashtable[index] = new_node;
        }
        
    }
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (word_count > 0)
    return word_count;
    else
    return 0;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for (int index = 0; index < HASHTABLE_SIZE; index++)
    {
        node* cursor = hashtable[index];
        while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor -> next;
            free(temp);
        }
    }
    return true;
}
