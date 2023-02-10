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

char *getParent(SpeciesInfo *speciesInfo, char *id){
    char *parent =NULL;
    for(int i =0; i<FILE_SIZE -1; i++){
        if(strcmp(speciesInfo[i].id, id)==0){
            return speciesInfo[i].name;
        }
    }
    return parent; 
}

char *getLineage(SpeciesInfo *speciesInfo, char *id, char lineage[MAX_SIZE], char *parentTarget){
    char lignee[MAX_SIZE];
    lignee[0] = '\0';
    for(int i=0; i<FILE_SIZE-1; i++){
        if(strcmp(id, speciesInfo[i].id)==0){
            strcpy(lignee, "\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":\"");
            strcat(lignee, speciesInfo[i].id);
            strcat(lignee,"\",\n\t\t\t\t\t\t\"name\":\"");
            strcat(lignee, speciesInfo[i].name);
            strcat(lignee,"\"\n\t\t\t\t\t },\n");
            strcat(lignee, lineage);
            strcpy(lineage, lignee);

            if(strcmp(speciesInfo[i].id_parent, "")==0){
                char lignee2[MAXI_SIZE]="\",\n\t\t\t\t\t\"parent\":\"";
                strcat(lignee2,parentTarget);
                strcat(lignee2, "\",\n\t\t\t\t\t\"lineage\":[\n\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":\"1\"");
                strcat(lignee2,",\n\t\t\t\t\t\t\"name\":\"root\"\n\t\t\t\t\t },\n");
                strcat(lignee2, lineage);
                strcpy(lineage, lignee2);
                break;
            }
            else{
                lineage = getLineage(speciesInfo, speciesInfo[i].id_parent, lineage, parentTarget);
            }
        }
    }
    //ajout du lineage

    return strdup(lineage);
}

/// @brief reading the taxonomy file to get the lineage
/// @param buffer 
/// @param species 
/// @return 
char *createLineage(SpeciesInfo *speciesInfo,char *species)
{
    char lineage[MAX_SIZE];
  //  printf("l'espèce est %s\n", species);
    for(int i =0; i<FILE_SIZE-1;i++){
       // 

        if(strcmp(species, speciesInfo[i].name)==0){
           // printf("on arrive là? %s\n",speciesInfo[i].name);
            strcpy(lineage, "\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":\"");
            strcat(lineage, speciesInfo[i].id);
            strcat(lineage,"\",\n\t\t\t\t\t\t\"name\":\"");
            strcat(lineage, speciesInfo[i].name);
            strcat(lineage,"\"\n\t\t\t\t\t }\n\t\t\t\t\t]\n\t\t\t\t}],");
            char *parent =getParent(speciesInfo, speciesInfo[i].id_parent);
            strcpy(lineage,getLineage(speciesInfo, speciesInfo[i].id_parent , lineage, parent));//ajout du parent 
            //il faut qu'on récupère le parent depuis la lignée
            break;
        }
    }  
    return strdup(lineage);
}