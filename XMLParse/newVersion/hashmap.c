#include "hashmap.h"
#include "lineage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fonction de hachage basique pour les chaînes de caractères
unsigned int hash(const char *key)
{
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        hash = hash * 31 + key[i]; // La multiplication par 31 est utilisée car peut être implémentée efficacement à l'aide d'un simple décalage de bits.
    }
    return hash % SPECIES_SIZE;
}
// fonction pour insérer une nouvelle paire clé-valeur dans la hashmap
void insert(Hashmap *hashmap, const char *key, int value, char *key2)
{
    
    // calculer l'index dans le tableau en utilisant la fonction de hachage
    unsigned int index = hash(key);
    // récupérer l'élément correspondant dans le tableau
    Entry *entry = hashmap->entries[index];
    // parcourir la liste chaînée pour trouver l'élément correspondant à la clé donnée
    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0 && strcmp(entry->key2, key2) == 0)
        {
            // si la clé existe déjà, mettre à jour sa valeur et retourner
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    // si la clé n'existe pas, créer un nouvel élément et l'ajouter à la liste chaînée
    Entry *new_entry = (Entry *)malloc(sizeof(Entry));
    new_entry->key = strdup(key);
    new_entry->key2 = strdup(key2);
    new_entry->value = value;
    new_entry->next = hashmap->entries[index];
    hashmap->entries[index] = new_entry;
}

// fonction pour créer une nouvelle hashmap
Hashmap *createHashMap(char *buffer)
{
    Hashmap *hashmap = (Hashmap *)malloc(sizeof(Hashmap));
    char id_species[MIN_SIZE], name_species[MAX_SIZE], other_name_species[MAX_SIZE];
    char *line = strtok(strdup(buffer), "\n");
    line = strtok(NULL, "\n");

    while (line != NULL)
    {
        sscanf(line, "%[^	]	%*[^	]	%[^	]	%*[^\n]", id_species, name_species);
       int len = strlen(line);
        int iteration = 0;
        for (int j = 0; j < len; j++)
        {
            if (line[j] == '\t')
            {
                iteration++;
                if (iteration == 5)
                {
                    int k = 0;
                    for (int l = j + 1; l < len; l++)
                    {
                        if (line[l] == '\t')
                        {
                            other_name_species[k] = '\0';
                            break;
                        }
                        else
                        {
                            other_name_species[k] = line[l];
                        }
                        k++;
                    }
                }
            }
        }
        insert(hashmap, name_species, atoi(id_species), other_name_species);
        line = strtok(NULL, "\n");
    }
    return hashmap;
}

// fonction pour récupérer la valeur associée à une clé donnée
int get(Hashmap *hashmap, const char *key)
{
     // calculate the index in the array using the hash function
    unsigned int index = hash(key);
    // get the corresponding element in the array
    Entry *entry = hashmap->entries[index];
    // traverse the linked list to find the element corresponding to the given key
    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
        {
            // if the corresponding element is found, return its value
            return entry->value;
        }
        else{
            //condition to know if the key2 is present in the entry->key2 
            char *name_species = strtok(strdup(entry->key2), ", ");
            while (name_species != NULL)
            {
                if (strcmp(name_species, key) == 0)
                {
                    return entry->value;
                }
                name_species = strtok(NULL, ", ");
            }
        }
        entry = entry->next;
    }
    // if the corresponding element is not found, return -1
    return -1;
}
