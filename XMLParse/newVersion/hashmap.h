#ifndef HASHMAP_H
#define HASHMAP_H
#include "parameters.h"


// la structure pour chaque élément de la hashmap
typedef struct Entry
{
    char *key;          // la clé
    char *key2;
    int value;          // la valeur associée à la clé
    struct Entry *next; // le pointeur vers l'élément suivant dans la liste chaînée

} Entry;

// la structure de la hashmap
typedef struct Hashmap
{
    Entry *entries[SPECIES_SIZE];
} Hashmap;

unsigned int hash(const char *key);
void insert(Hashmap *hashmap, const char *key, int value, char *key2);
Hashmap *createHashMap(char *buffer);
int get(Hashmap *hashmap, const char *key);


#endif