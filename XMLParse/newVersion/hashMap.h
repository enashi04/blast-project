#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE 8192
#define CAPACITY 2429415 //nbre de ligne du fichier taxo


//on définit les items
typedef struct Ht_item Ht_item;
struct Ht_item { //modifier la hashmap
    char* key;
    char* value;
};

//on définit la hashtable
typedef struct HashTable HashTable;
struct HashTable {
    // Contains an array of pointers to items
    Ht_item** items;
    int size;
    int count;
};

int hash_index(HashTable *table, char *key);


//création d'un item
Ht_item* create_item(char* key, char* value);

//création d'une hashtable
HashTable* create_table(int size) ;

// Frees an item
void free_item(Ht_item* item) ;

//frees the table
void free_table(HashTable* table) ;

//insérer l'item dans la hashmap
void ht_insert(HashTable* table, char* key, char* value) ;

char* ht_search(HashTable* table, char* key) ;
//passer de void à char car ici on veut récupérer la valeur  
void print_search(HashTable* table, char* key) ;

void print_table(HashTable* table);
