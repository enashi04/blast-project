#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lineage.h"
#include "parameters.h"

/// @brief Stock the taxonomy file in memory
/// @param buffer 
/// @param filename 
/// @return 
char *makebuffer(char *filename)
{
    char *buffer;
    //Open the taxonomy file
    FILE *file=fopen(filename, "r");
    //check of the file existence
    if (!file)
    {
        printf("Unable to open file");
        exit(1);
    }
    //move to the end of the file
    fseek(file, 0, SEEK_END);
    //get the length of the file
    long fileSize = ftell(file);
    //return to the beginning of the file
    rewind(file);
    //initialize the buffer
    buffer = (char *)malloc((fileSize + 1) * sizeof(char));
    //fill the buffer with the contents of the file
    fread(buffer, sizeof(char), fileSize, file);
    //don't forget to put the '\0' at the end of the buffer
    buffer[fileSize] = '\0';
    return buffer;
}


/// @brief get Lineage of the species
/// @param buffer 
/// @param speciesID 
/// @param lineage 
/// @return 
char *getLineage(char *buffer, char *speciesID, char *lineage)
{
    char *line = strtok(strdup(buffer), "\n");
    //Declaration of the variables for taxid, name, rank and parents of the species
    char id_species[MIN_SIZE], name_species[MIN_SIZE], rank_species[MIN_SIZE], id_parent_species[MIN_SIZE];
    //Declare this variable to fill and copy paste lineage
    char lignee[MAX_SIZE];

    //browse the buffer
    while (line != NULL)
    {
        sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", id_species, name_species, rank_species, id_parent_species);
        //check if the id in parameter is the same as the id in the file
        if (strcmp(speciesID, id_species) == 0)
        {
            strcpy(lignee, name_species);
            strcat(lignee, "/");
            strcat(lignee, lineage);
            //fill lineage
            strcpy(lineage, lignee);
            //check if the rank of the species is superkingdom or phylum => stop the recursive function
            if(strcmp(rank_species, "superkingdom")==0 || strcmp(rank_species, "phylum")==0){
                return lineage;
            }
            else{
                //recursivity
                getLineage(buffer, id_parent_species, lineage);
            }
        }
        line = strtok(NULL, "\n");
    }
    return lineage;
}

/// @brief get the parent Name of the species
/// @param buffer 
/// @param parentID 
/// @return 
// char *getParentName(char *buffer,char *parentID){
//     //initialize the parentName to parentId if we could'nt find it.
//     char *parentName=parentID;
//     char *line = strtok(strdup(buffer), "\n");
//     //Declaration of the variables for taxid, name, rank and parents of the species
//     char id_species[MIN_SIZE], name_species[MIN_SIZE], rank_species[MIN_SIZE], id_parent_species[MIN_SIZE];
//     //browse the buffer
//     while (line != NULL)
//     {
//         sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", id_species, name_species, rank_species, id_parent_species);
//         //check 
//         if (strcmp(parentID, id_species) == 0)
//         {
//             //we copy the name of the parent we found
//             strcpy(parentName, name_species);
//             break;
//         }
//         line = strtok(NULL, "\n");
//     }
//     return parentName;
// }

/// @brief reading the taxonomy file to get the lineage
/// @param buffer 
/// @param species 
/// @return 
char *readTaxoFile(char *buffer,char *species)
{
    char *line = strtok(strdup(buffer), "\n");
    //déclaration des variables afin de récupérer chaque ligne de taxofile
    char id_species[MIN_SIZE], name_species[MIN_SIZE], rank_species[MIN_SIZE], id_parent_species[MIN_SIZE];
    //char lineage[4096]; //pb de taille avec 4096 ca marche
    char *lineage = (char *)malloc(4096*sizeof(char)); //pb de taille
    //browse the buffer
    while (line != NULL)
    {
        sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", id_species, name_species, rank_species, id_parent_species);
        //check if the name in parameter is the same as the name in the file
        if (strcmp(name_species, species) == 0)
        {
            //check the rank of the species
            if (strcmp(rank_species, "superkingdom") == 0 || strcmp(rank_species, "phylum")==0)
            {
                lineage = name_species;
                break;
            }
            else
            {
                //we copy the name of the species in lineage and it's the beginning of the lineage
                strcpy(lineage, name_species);
                //call the function getLineage to get the complete lineage of the species
                char *tmpResult = getLineage(buffer, id_parent_species, lineage); 
                //copy the result in lineage
                memmove(lineage, tmpResult, strlen(tmpResult) + 1);
                break;
            }
        }
        line = strtok(NULL, "\n");
    }
    return lineage;
}