#include "hashmap.h"
#include "lineage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int hash(const char *key)
{
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        if(key[i] == ','){
            break;
        }
        // La multiplication par 31 est utilisée car peut être implémentée efficacement à l'aide d'un simple décalage de bits.
        hash = hash * 31 + key[i]; 
    }
    return hash % SPECIES_SIZE;
}

// fonction pour insérer une nouvelle paire clé-valeur dans la hashmap
void insert(Hashmap *hashmap, const char *key, int value)
{
    // calculer l'index dans le tableau en utilisant la fonction de hachage
    unsigned int index = hash(key);
    // récupérer l'élément correspondant dans le tableau
    Entry *entry = hashmap->entries[index];
    // parcourir la liste chaînée pour trouver l'élément correspondant à la clé donnée
    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
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
    new_entry->value = value;
    new_entry->next = hashmap->entries[index];
    hashmap->entries[index] = new_entry;
    
}

void insert_othername(Hashmap *hashmap, const char *name, const char *value){
    //parcourir la chaine de caractère name
    int len = strlen(name);
    char *new_name;
    for(int i = 0; i < len; i++){
        if(name[i] == ',' && name[i+1]==' '){
            new_name = (char *)malloc((i+1) * sizeof(char));
            for(int j = 0; j < i; j++){
                new_name[j] = name[j];
            }
            new_name[i] = '\0';
            insert(hashmap, new_name, atoi(value));
            free(new_name);
        }
    }
}

// fonction pour créer une nouvelle hashmap
Hashmap *createHashMap(char *buffer)
{
    Hashmap *hashmap = (Hashmap *)malloc(sizeof(Hashmap));
    char id_species[MIN_SIZE], name_species[MAX_SIZE], othername_species[MAXI_SIZE];
    char *line = strtok(strdup(buffer), "\n");
    line = strtok(NULL, "\n");
    while (line != NULL)
    {
        sscanf(line, "%[^	]	%*[^	]	%[^	]	%*[^\n]", id_species, name_species);
        int len = strlen(line), iteration=0;
        // Boucle pour récupérer le other name
        for (int i = 0; i < len; i++)
        {
            if(line[i]=='\t'){
                iteration ++;
                if(iteration==5){
                    int j=0;
                    for(int k=i+1; k<len; k++){
                        if (line[k] == '\t')
                        {
                            othername_species[j] = '\0';
                            //faire appel à une autre fonction ici 
                            insert_othername(hashmap, othername_species, id_species);
                            break;
                        }
                        else
                        {
                            othername_species[j] = line[k];
                        }
                        j++;
                    }
                }
            }  
        }
        insert(hashmap, name_species, atoi(id_species));
        line = strtok(NULL, "\n");
    }
    return hashmap;
}

// fonction pour récupérer la valeur associée à une clé donnée
int get(Hashmap *hashmap, const char *key)
{
    unsigned int index = hash(key);
    Entry *entry = hashmap->entries[index];
    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
        {
            return entry->value;
        }
        entry = entry->next;
     }
    return -1;
}

// int main(int argc, char **argv){
//     // // créer la hashmap
//     Hashmap *hashmap = createHashMap(makebuffer("taxonomy.dat"));
//     // récupérer la valeur de Anser cygnoides
//     int value = get(hashmap, "Anser cygnoides");
//     printf("Anser cygnoides: %d\n", value);
 
// } 