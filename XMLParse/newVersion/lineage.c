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
    char id_species[MIN_SIZE], name_species[MIN_SIZE], id_parent_species[MIN_SIZE];
    char lignee[MAX_SIZE];

    while (line != NULL)
    {
        sscanf(line, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species, name_species);
        if (strcmp(speciesID, id_species) == 0)
        {
             
            strcat(lignee, "\t\t\t\t {\n\t\t\t\t\t\"taxid\":\"");
            strcat(lignee, id_species);
            strcat(lignee,",\n\t\t\t\t\t\"name\":\"");
            strcat(lignee, name_species);
            strcat(lignee,"\n\t\t\t\t },\n");
            strcat(lignee, lineage);
            strcpy(lineage, lignee);

            int len = strlen(line);
            int iteration = 0;
            for (int i = 0; i < len + 1; i++)
            {
                if (line[i] == '	')
                {
                    iteration++;
                }
                if (iteration == 9)
                {
                    int k = 0;
                    for (int j = i + 1; j < len; j++)
                    {
                        if (line[j] == '	')
                        {
                            id_parent_species[k] = '\0';
                            break;
                        }
                        else
                        {
                            id_parent_species[k] = line[j];
                        }
                        k++;
                    }
                    break;
                }
            }
            if(strcmp(id_parent_species,"")==0){
                char lignee2[MAX_SIZE];
                strcat(lignee2, "\t\t\t\t {\n\t\t\t\t\t\"taxid\":\"1\"");
                strcat(lignee2,",\n\t\t\t\t\t\"name\":\"root\"\n\t\t\t\t },\n");
                strcat(lignee2, lineage);
                strcpy(lineage, lignee2);
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


/// @brief reading the taxonomy file to get the lineage
/// @param buffer 
/// @param species 
/// @return 
char *readTaxoFile(char *buffer,char *species)
{
    char *line = strtok(strdup(buffer), "\n");
    char id_species[MIN_SIZE], name_species[MIN_SIZE], id_parent_species[MIN_SIZE];
    char *lineage = (char *)malloc(4096*sizeof(char)); 
    while(line!=NULL)
    {
       sscanf(line, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species, name_species);
        if (strcmp(name_species, species) == 0)
        {
            
            strcat(lineage, "\t\t\t\t {\n\t\t\t\t\t\"taxid\":\"");
            strcat(lineage, id_species);
            strcat(lineage,",\n\t\t\t\t\t\"name\":\"");
            strcat(lineage, name_species);
            strcat(lineage,"\n\t\t\t\t }");

            int len = strlen(line);
            int iteration = 0;
            for (int i = 0; i < len + 1; i++)
            {
                if (line[i] == '	')
                {
                    iteration++;
                }
                if (iteration == 9)
                {
                    int k = 0;
                    for (int j = i + 1; j < len; j++)
                    {
                        if (line[j] == '	')
                        {
                            id_parent_species[k] = '\0';
                            break;
                        }
                        else
                        {
                            id_parent_species[k] = line[j];
                        }
                        k++;
                    }
                    break;
                }
            }
            char *tmpResult = getLineage(buffer, id_parent_species, lineage); 
            memmove(lineage, tmpResult, strlen(tmpResult) + 1);
            break;
        }
        line = strtok(NULL, "\n");
    }
    return lineage;
}
