#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameters.h"

/// @brief Stock the taxonomy file in memory
/// @param buffer
/// @param filename
/// @return
char *makebuffer(char *filename)
{
    char *buffer;
    // Open the taxonomy file
    FILE *file = fopen(filename, "r");
    // check of the file existence
    if (!file)
    {
        printf("Unable to open file");
        exit(1);
    }
    // move to the end of the file
    fseek(file, 0, SEEK_END);
    // get the length of the file
    long fileSize = ftell(file);
    // return to the beginning of the file
    rewind(file);
    // initialize the buffer
    buffer = (char *)malloc((fileSize + 1) * sizeof(char));
    // fill the buffer with the contents of the file
    fread(buffer, sizeof(char), fileSize, file);
    // don't forget to put the '\0' at the end of the buffer
    buffer[fileSize] = '\0';
    return buffer;
}

// la structure pour chaque élément de la hashmap
typedef struct Entry
{
    char *key;          // la clé
    int value;          // la valeur associée à la clé
    char *valueString;  // la valeur de la clé en String 
    struct Entry *next; // le pointeur vers l'élément suivant dans la liste chaînée
} Entry;

// la structure de la hashmap
typedef struct Hashmap
{
    Entry *entries[SPECIES_SIZE];
} Hashmap;

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
void insert(Hashmap *hashmap, const char *key, int value, char *valueString)
{
    // calculer l'index dans le tableau en utilisant la fonction de hachage
    unsigned int index = hash(key);
    // récupérer l'élément correspondant dans le tableau
    Entry *entry = hashmap->entries[index];
    // parcourir la liste chaînée pour trouver l'élément correspondant à la clé donnée
    while (entry != NULL)
    {
        if (strstr(entry->key, key) !=NULL)
        {
            // si la clé existe déjà, mettre à jour sa valeur et retourner
            entry->value = value;
            entry->valueString = valueString;
            return;
        }
        entry = entry->next;
    }
    // si la clé n'existe pas, créer un nouvel élément et l'ajouter à la liste chaînée
    Entry *new_entry = (Entry *)malloc(sizeof(Entry));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->valueString = valueString;
    new_entry->next = hashmap->entries[index];
    hashmap->entries[index] = new_entry;
}


// fonction pour créer une nouvelle hashmap
Hashmap *createHashMap()
{
    Hashmap *hashmap = (Hashmap *)malloc(sizeof(Hashmap));
    char id_species[MIN_SIZE], name_species[MAX_SIZE], other_name_species[MAXI_SIZE];
    char *buffer = makebuffer(FICHIER);
    char *line = strtok(strdup(buffer), "\n");
    line = strtok(NULL, "\n");
    int len = strlen(line);
    int iteration = 0;
    while (line != NULL)
    {
        sscanf(line, "%[^	]	%*[^	]	%[^	]	%*[^\n]", id_species, name_species);
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
    free(buffer);
    return hashmap;
}


// fonction pour récupérer la valeur associée à une clé donnée
int get(Hashmap *hashmap, const char *key) 
{
    // calculer l'index dans le tableau en utilisant la fonction de hachage
    
    unsigned int index = hash(key);
    // récupérer l'élément correspondant dans le tableau
    Entry *entry = hashmap->entries[index];
    // parcourir la liste chaînée pour trouver l'élément correspondant à la clé donnée
    
    while (entry != NULL)
    {   
        if(strcmp(entry->key, key) == 0){
            return entry->value;
        }
        else{
            char *token = strtok(strdup(entry->valueString), ",");
            while(token!=NULL){   
                if(token[0]==' '){
                    memmove(token, token + 1, strlen(token) - 1);
                }
                printf("token is %s\n", token);
                if(strcmp(token, key) == 0){
                    printf("est ce qu'on rentre ici ?\n");
                    printf("la valeur est %u et la clé est %s\n", entry->value, entry->key);
                    return entry->value;
                }
                token=strtok(NULL,",");
            }
        } 
            
        entry = entry->next;
    }
  
    // si l'élément correspondant n'est pas trouvé, retourner -1
    return -1;
}


int main(int argc, char **argv)
{
    Hashmap *hashmap = createHashMap();

    printf("la valeur pour la clé Homo Sapiens est %d\n", get(hashmap, "Homo sapiens"));
    printf("la valeur pour la clé Bacteria est %d\n", get(hashmap, "Monera"));
    printf("la valeur pour la clé vectors est %d\n", get(hashmap, "Saccharomyces cerevisiae S288C"));
    // free(buffer);
    free(hashmap);
}

// fonction pour supprimer une paire clé-valeur de la hashmap
// void delete(Hashmap* hashmap, const char* key) {
//     // calculer l'index dans le tableau en utilisant la fonction de hachage
//     unsigned int index = hash(key);

//     // récupérer l'élément correspondant dans le tableau
//     Entry* entry = hashmap->entries[index];
//     Entry* prev_entry = NULL;

//     // parcourir la liste chaînée pour trouver l'élément correspondant à la clé donnée
//     while (entry != NULL) {
//         if (strcmp(entry->key, key) == 0) {
//             // si l'élément correspondant est trouvé, le supprimer de la liste chaînée
//             if (prev_entry == NULL) {
//                 hashmap->entries[index] = entry->next;
//             } else {
//                 prev_entry->next = entry->next;
//             }

//             // libérer la mémoire allouée pour la clé et l'élément
//             free(entry->key);
//             free(entry);

//             return;
//         }
//         prev_entry = entry;
//         entry = entry->next;
//     }
// }