#include "hashmap.h"
#include "lineage.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// char *makebuffer(char *filename)
// {
//     char *buffer;
//     // Open the taxonomy file
//     FILE *file = fopen(filename, "r");
//     // check of the file existence
//     if (!file)
//     {
//         printf("Unable to open file taxonomy.dat");
//         exit(1);
//     }
//     // move to the end of the file
//     fseek(file, 0, SEEK_END);
//     // get the length of the file
//     long fileSize = ftell(file);
//     // return to the beginning of the file
//     rewind(file);
//     // initialize the buffer
//     buffer = (char *)malloc((fileSize + 1) * sizeof(char));
//     // fill the buffer with the contents of the file
//     fread(buffer, sizeof(char), fileSize, file);
//     // don't forget to put the '\0' at the end of the buffer
//     buffer[fileSize] = '\0';   
//     // close the file
//     fclose(file);
//     return buffer;
// }

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
    char new_str[MAXI_SIZE];
    int len = strlen(name);
    int k = 0;
    int debut = 0;
    // Boucle pour récupérer les noms séparés par une virgule
    for (int i = 0; i < len; i++)
    {
        //si on trouve une virgule
        if(name[i] == ',' && name[i+1] == ' '){
            //on remplit de k à i le mot
            for(int j =debut; j<i; j++){
                new_str[k] = name[j];
                k++;
            }
            //vérifions que le mot ne contient pas de parenthèse ou contient les deux parenthèses
            if( (strpbrk(new_str, "(")!=NULL && strpbrk(new_str, ")")!=NULL )|| strpbrk(new_str, "(")==NULL){
                new_str[k] = '\0';
                //ici on ajoute à la hashmap
                insert(hashmap, (const char *)new_str, atoi(value));
                k=0;
                debut =i+2;
                strcpy(new_str, "");
            }
            else{
                debut = i;
            }
        }
        if(i==len-1){
            for(int j =debut; j<i+1; j++){
                new_str[k] = name[j];
                k++;
            }
            new_str[k] = '\0';
            insert(hashmap, (const char *)new_str, atoi(value));
            strcpy(new_str, "");
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
                            //printf("on arrive làààààà\n");
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
    INFO("Hashmap created");
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
//     char *str = "Streptomyces sp. NEAU-TXT10, Streptomyces bryophytorum Li et al. 2016, Streptomyces sp. NEAU-HZ10, CGMCC 4.7151, DSM 42138, strain NEAU-HZ10, Actinacidiphila bryophytorum (Li et al. 2016) Madhaiyan et al. 2022, Streptomyces bryophytorum";
//     char new_str[MAXI_SIZE];
//     int len = strlen(str);
//     int k = 0;
//     int debut = 0;
//     // Boucle pour récupérer les noms séparés par une virgule
//     for (int i = 0; i < len; i++)
//     {
//         //si on trouve une virgule
//         if(str[i] == ',' && str[i+1] == ' '){
//             //on remplit de k à i le mot
//             for(int j =debut; j<i; j++){
//                 new_str[k] = str[j];
//                 k++;
//             }
//             //vérifions que le mot ne contient pas de parenthèse ou contient les deux parenthèses
//             if( (strpbrk(new_str, "(")!=NULL && strpbrk(new_str, ")")!=NULL )|| strpbrk(new_str, "(")==NULL){
//                 new_str[k] = '\0';
//                 printf("mot : %s\n", new_str);
//                 strcpy(new_str, "");
//                 //ici on ajoute à la hashmap
//                 k=0;
//                 debut =i+2;
//             }
//             else{
//                 //on recommence la boucle
//                 debut = i;
//             }
//         }
//         if(i==len-1){
//             for(int j =debut; j<i+1; j++){
//                 new_str[k] = str[j];
//                 k++;
//             }
//             new_str[k] = '\0';
//             printf("mot : %s\n", new_str);
//             strcpy(new_str, "");
//         }
//     }
// }