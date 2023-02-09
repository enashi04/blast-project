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

void getLineage(SpeciesInfo *speciesInfo, char *id, char lineage[MAX_SIZE]){
    char lignee[MAX_SIZE];
    lignee[0] = '\0';
    for(int i=0; i<FILE_SIZE; i++){
        if(strcmp(id, speciesInfo[i].id)==0){

            strcat(lignee, "\t\t\t\t {\n\t\t\t\t\t\"taxid\":\"");
            strcat(lignee, speciesInfo[i].id);
            strcat(lignee,",\n\t\t\t\t\t\"name\":\"");
            strcat(lignee, speciesInfo[i].name);
            strcat(lignee,"\n\t\t\t\t },\n");
            strcat(lignee, lineage);
            strcpy(lineage, lignee);

            if(strcmp(speciesInfo[i].id_parent, "")==0){
                char lignee2[MAXI_SIZE];
                strcat(lignee2, "\t\t\t\t {\n\t\t\t\t\t\"taxid\":\"1\"");
                strcat(lignee2,",\n\t\t\t\t\t\"name\":\"root\"\n\t\t\t\t },\n");
                strcat(lignee2, lineage);
                strcpy(lineage, lignee2);
                break;
            }
            else{
                getLineage(speciesInfo, speciesInfo[i].id_parent, lineage);
            }
        }
    }
    printf("on arrive ici un jour\n");
}

/// @brief reading the taxonomy file to get the lineage
/// @param buffer 
/// @param species 
/// @return 
char *createLineage(SpeciesInfo *speciesInfo,char *id_parent)
{
    char lineage[MAX_SIZE];
    char *parent_name=NULL;
    for(int i =0; i<FILE_SIZE;i++){
        if(strcmp(id_parent, speciesInfo[i].id)==0){
            parent_name = speciesInfo[i].name;
            char *parent_id = speciesInfo[i].id_parent;
            // strcpy(lineage, "\t\t\t\t {\n\t\t\t\t\t\"taxid\":\"");
            // strcat(lineage, speciesInfo[i].id);
            // strcat(lineage,",\n\t\t\t\t\t\"name\":\"");
            // strcat(lineage, speciesInfo[i].name);
            // strcat(lineage,"\n\t\t\t\t }");
            getLineage(speciesInfo, parent_id , lineage); 
            printf("%s\n", lineage);
            //memmove(lineage, tmpResult, strlen(tmpResult) + 1);
            break;
        }
    }  
    //rajouter le parent hein ! 
    char finalOut[MAX_SIZE] ;
    strcpy(finalOut, "\t\t\t\t\"parent\":\"");
    strcat(finalOut, parent_name);
    strcat(finalOut,"\",\n\t\t\t\t\"lineage\":[\n");
    strcat(finalOut,lineage);
    
    return strdup(finalOut);
}

int main(int argc, char **argv){
    char *buffer = makebuffer(FICHIER);
    SpeciesInfo *test = fillStructure(buffer);
    char *lineage =createLineage(test, "1773");
    printf("lineage :%s\n", lineage);
    return 0;
}