#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lineage.h"
#include "blastInfo.h"

char *getRank(char *line)
{
    char rank_species[MIN_SIZE];
    int len = strlen(line);
    int iteration = 0;
    for (int i = 0; i < len + 1; i++)
    {
        if (line[i] == '	')
        {
            iteration++;
        }
        if (iteration == 8)
        {
            int k = 0;
            for (int j = i + 1; j < len; j++)
            {
                if (line[j] == '	')
                {
                    rank_species[k] = '\0';
                    break;
                }
                else
                {
                    rank_species[k] = line[j];
                }
                k++;
            }
            break;
        }
    }
    return strdup(rank_species);
}

SpeciesInfo *fillStructure(char *buffer)
{
    SpeciesInfo *outInfo = (SpeciesInfo *)malloc(FILE_SIZE * sizeof(SpeciesInfo));

    char *line = strtok(strdup(buffer), "\n");
    line = strtok(NULL, "\n");

    for (int it = 0; it < FILE_SIZE && line != NULL; it++) {
        char id_species[MIN_SIZE], name_species[MIN_SIZE], id_parent_species[MIN_SIZE];
        sscanf(line, "%[^\t]\t%*[^\t]\t%[^\t]\t%*[^\n]", id_species, name_species);

        int len = strlen(line);
        int iteration = 0;
        for (int i = 0; i < len + 1; i++) {
            if (line[i] == '\t') {
                iteration++;
            }
            if (iteration == 9) {
                int k = 0;
                for (int j = i + 1; j < len; j++) {
                    if (line[j] == '\t') {
                        id_parent_species[k] = '\0';
                        break;
                    } else {
                        id_parent_species[k] = line[j];
                    }
                    k++;
                }
                break;
            }
        }

        outInfo[it].id = strdup(id_species);
        outInfo[it].name = strdup(name_species);
        outInfo[it].rank = getRank(line);
        outInfo[it].id_parent = strdup(id_parent_species);
        line = strtok(NULL, "\n");
    }

    return outInfo;
}

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

char *getLineage(SpeciesInfo *speciesInfo, char *id, char lineage[MAX_SIZE]){
    char lignee[MAX_SIZE];
    lignee[0] = '\0';
    for(int i=0; i<FILE_SIZE-1; i++){
        if(strcmp(id, speciesInfo[i].id)==0){
            printf("123");
            strcpy(lignee, "\t\t\t\t {\n\t\t\t\t\t\"taxid\":\"");
            strcat(lignee, speciesInfo[i].id);
            strcat(lignee,",\n\t\t\t\t\t\"name\":\"");
            strcat(lignee, speciesInfo[i].name);
            strcat(lignee,"\n\t\t\t\t },\n");
            strcat(lignee, lineage);
            strcpy(lineage, lignee);

            if(strcmp(speciesInfo[i].id_parent, "")==0){
                char lignee2[MAXI_SIZE];
                strcpy(lignee2, "\t\t\t\t {\n\t\t\t\t\t\"taxid\":\"1\"");
                strcat(lignee2,",\n\t\t\t\t\t\"name\":\"root\"\n\t\t\t\t },\n");
                strcat(lignee2, lineage);
                strcpy(lineage, lignee2);
                printf("lineage is %s\n", lignee2);
                break;
            }
            else{
                lineage = getLineage(speciesInfo, speciesInfo[i].id_parent, lineage);
            }
        }
    }
    return strdup(lineage);
}

/// @brief reading the taxonomy file to get the lineage
/// @param buffer 
/// @param species 
/// @return 
char *createLineage(SpeciesInfo *speciesInfo,char *species)
{
    char lineage[MAX_SIZE];
    for(int i =0; i<FILE_SIZE;i++){
        if(strcmp(species, speciesInfo[i].name)==0){
            strcpy(lineage, "\t\t\t\t {\n\t\t\t\t\t\"taxid\":\"");
            strcat(lineage, speciesInfo[i].id);
            strcat(lineage,",\n\t\t\t\t\t\"name\":\"");
            strcat(lineage, speciesInfo[i].name);
            strcat(lineage,"\n\t\t\t\t }");
            strcpy(lineage,getLineage(speciesInfo, speciesInfo[i].id_parent , lineage)); 
            break;
        }
    }  
    return strdup(lineage);
}

int main(int argc, char **argv){
    char *buffer = makebuffer(FICHIER);
    SpeciesInfo *test = fillStructure(buffer);
    char *lineage =createLineage(test, "Mycobacterium tuberculosis");
    printf("lineage :%s\n", lineage);
    return 0;
}