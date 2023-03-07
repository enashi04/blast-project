#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lineage.h"
#include "blastInfo.h"
#include "fillStructure.h"

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

// char *getParent(SpeciesInfo *speciesInfo, int id){
//     char *parent =NULL;
//     for(int i =0; i<FILE_SIZE -1; i++){
//         if(strcmp(speciesInfo[i].id, id)==0){
//             return speciesInfo[i].name;
//         }
//     }
//     return parent;
// }

char *getLineage(SpeciesInfo *speciesInfo, int id, char lineage[MAX_SIZE], int parentTarget)
{
    char lignee[MAXI_SIZE];
    // lignee[0] = '\0';
    strcpy(lignee, "\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":\"");
    char str_id[MIN_SIZE];
    sprintf(str_id, "%d", id);
    strcat(lignee, str_id);
    strcat(lignee, "\",\n\t\t\t\t\t\t\"name\":\"");
    strcat(lignee, speciesInfo[id].name);
    strcat(lignee, "\"\n\t\t\t\t\t },\n");
    strcat(lignee, lineage);
    strcpy(lineage, lignee);

    if (speciesInfo[id].parentid == 0)
    {
        char lignee2[MAXI_SIZE] = "\t\t\t\t\t\"parent\":\"";
        strcat(lignee2, speciesInfo[parentTarget].name);
        strcat(lignee2, "\",\n\t\t\t\t\t\"lineage\":[\n\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":\"1\"");
        strcat(lignee2, ",\n\t\t\t\t\t\t\"name\":\"root\"\n\t\t\t\t\t },\n");
        strcat(lignee2, lineage);
        strcpy(lineage, lignee2);
    }
    else
    {
        lineage = getLineage(speciesInfo, speciesInfo[id].parentid, lineage, parentTarget);
    }
    return strdup(lineage);
}

char *createLineage(SpeciesInfo *speciesInfo, char *species)
{
    char *lineage = malloc(sizeof(char) * MAXI_SIZE);
    for (int i = 0; i < SPECIES_SIZE; i++)
    {
        char *espece = (char *)malloc(sizeof(char) * MIN_SIZE);
        strcpy(espece, species);
        strcat(espece, ",");
        char *check_string = strstr(speciesInfo[i].othername, espece);
        if (strcmp(species, speciesInfo[i].name) == 0)
        {
            // printf("on rentre ici au moins une fois\n");
            strcpy(lineage, "\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":\"");
            char str_i[MIN_SIZE];
            sprintf(str_i, "%d", i);
            strcat(lineage, str_i);
            strcat(lineage, "\",\n\t\t\t\t\t\t\"name\":\"");
            strcat(lineage, speciesInfo[i].name);
            strcat(lineage, "\"\n\t\t\t\t\t }\n\t\t\t\t\t]\n\t\t\t\t}],");
            char *returnLineage = getLineage(speciesInfo, speciesInfo[i].parentid, lineage, speciesInfo[i].parentid);
            strcpy(lineage, returnLineage);
            // il faut qu'on récupère le parent depuis la lignée
            break;
        }
        else if (check_string != NULL)
        {
            printf("%s -> %s\n", species, check_string);
            strcpy(lineage, "\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":\"");
            char str_i[MIN_SIZE];
            sprintf(str_i, "%d", i);
            strcat(lineage, str_i);
            strcat(lineage, "\",\n\t\t\t\t\t\t\"name\":\"");
            strcat(lineage, speciesInfo[i].name);
            strcat(lineage, "\"\n\t\t\t\t\t }\n\t\t\t\t\t]\n\t\t\t\t}],");
            // char *parent =getParent(speciesInfo, speciesInfo[i].id_parent); //2nd parameter : int
            char *returnLineage = getLineage(speciesInfo, speciesInfo[i].parentid, lineage, speciesInfo[i].parentid);
            strcpy(lineage, returnLineage); // ajout du parent //Second parameter : int
            break;
        }
    }
    return strdup(lineage);
}
