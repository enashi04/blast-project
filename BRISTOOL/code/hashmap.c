#include "hashmap.h"
#include "lineage.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



unsigned int hash(const char *key)
{
    //hash function used to calculate the index of the array
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        if(key[i] == ','){
            break;
        }
        //the multiplication by 31 is used because it can be implemented efficiently using a simple bit shift.
        hash = hash * 31 + key[i]; 
    }
    return hash % SPECIES_SIZE;
}

//fnction to insert a new key-value pair into the hashmap
void insert(Hashmap *hashmap, const char *key, int value)
{
    //get the index of the key 
    unsigned int index = hash(key);
    //get the element corresponding to the index
    Entry *entry = hashmap->entries[index];
    //browse the linked list to find the element corresponding to the given key
    while (entry != NULL)
    {
        //if the key already exists, update its value and return
        if (strcmp(entry->key, key) == 0)
        {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    //otherwise, create a new element and add it to the linked list
    Entry *new_entry = (Entry *)malloc(sizeof(Entry));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = hashmap->entries[index];
    hashmap->entries[index] = new_entry;
}

//function to insert a new key-value pair into the hashmap (othername of the species)
void insert_othername(Hashmap *hashmap, const char *name, const char *value){
    char new_str[MAXI_SIZE];
    int len = strlen(name);
    int k = 0;
    int debut = 0;
    // loop to get the name of the species
    for (int i = 0; i < len; i++)
    {
        //there is a comma and a space
        if(name[i] == ',' && name[i+1] == ' '){
            //fill the word from k to i
            for(int j =debut; j<i; j++){
                new_str[k] = name[j];
                k++;
            }

            //verify that the word does not contain parentheses or contains both parentheses
            if( (strpbrk(new_str, "(")!=NULL && strpbrk(new_str, ")")!=NULL )|| strpbrk(new_str, "(")==NULL){
                new_str[k] = '\0';
                //add the key-value pair to the hashmap
                insert(hashmap, (const char *)new_str, atoi(value));
                k=0;
                debut =i+2;
                //empty the string
                new_str[0]='\0';
            }
            //the word contains one parenthesis, so we need to continue the filling of the word
            else{
                debut = i;
            }
        }
        //the last caracter of the string
        if(i==len-1){
            //fill the word from k to i
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

//function to create a new hashmap
Hashmap *createHashMap(char *buffer)
{
    //initialize the hashmap
    Hashmap *hashmap = (Hashmap *)malloc(sizeof(Hashmap));
    //initialize the entries
    char id_species[MIN_SIZE], name_species[MAX_SIZE], othername_species[MAXI_SIZE];
    //get the first line of the file 
    char *line = strtok(strdup(buffer), "\n");
    line = strtok(NULL, "\n");
    //loop to get the names and value of the species 
    while (line != NULL)
    {
        //get the id and the name of the species
        sscanf(line, "%[^	]	%*[^	]	%[^	]	%*[^\n]", id_species, name_species);
        int len = strlen(line), iteration=0;
        // loop to get the other names of the species
        for (int i = 0; i < len; i++)
        {
            if(line[i]=='\t'){
                iteration ++;
                //position of the other names in the line
                if(iteration == 4){
                    int j=0;
                    //loop to get the other names
                    for(int k=i+1; k<len; k++){
                        if (line[k] == '\t')
                        {
                            othername_species[j] = '\0';
                            //add the key-value pair to the hashmap
                            insert_othername(hashmap, othername_species, id_species);
                            break;
                        }
                        else
                        {
                            // fill the othername_species string
                            othername_species[j] = line[k];
                        }
                        j++;
                    }
                }
                else if(iteration==5){
                    int j=0;
                    //loop to get the other names
                    for(int k=i+1; k<len; k++){
                        if (line[k] == '\t')
                        {
                            othername_species[j] = '\0';
                            //add the key-value pair to the hashmap
                            insert_othername(hashmap, othername_species, id_species);
                            break;
                        }
                        else
                        {
                            // fill the othername_species string
                            othername_species[j] = line[k];
                        }
                        j++;
                    }
                }
            }  
        }
        // insert the key-value pair into the hashmap
        insert(hashmap, name_species, atoi(id_species));
        //get the next line
        line = strtok(NULL, "\n");
    }
    INFO("Hashmap created");
    return hashmap;
}

//function to get the value of a given key
int get(Hashmap *hashmap, const char *key)
{
    //get the index of the key
    unsigned int index = hash(key);
    //get the element corresponding to the index
    Entry *entry = hashmap->entries[index];
    //browse the linked list to find the element corresponding to the given key
    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
        {
            return entry->value;
        }
        entry = entry->next;
     }
    //if the key does not exist, return -1
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