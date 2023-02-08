#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parameters.h"
#include "fillStructure.h"

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
    char *rank =rank_species;
    return rank;
}

SpeciesInfo *fillStructure(char *buffer)
{
    struct SpeciesInfo *outInfo= NULL;
    char *line = strtok(strdup(buffer), "\n");
    int it=0;
    while (line != NULL)
    {
        char id_species[MIN_SIZE], name_species[MIN_SIZE], id_parent_species[MIN_SIZE], parent_name[MIN_SIZE];
        //récupération du taxId et du nom
        sscanf(line, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species, name_species);

        //récupération du parent ID
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
        //reparcours du fichier pour récupérer son parent
        char *line2 = strtok(strdup(buffer), "\n");
        char id_species1[MIN_SIZE], name_species1[MIN_SIZE];

        while (line2 != NULL)
        {
            sscanf(line2, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species1, name_species1);
            if (strcmp(id_parent_species, id_species1) == 0)
            {
                //strcpy(parent_name, name_species1);
                break;
            }
            line2 = strtok(NULL, "\n");
        }

      
        outInfo=(SpeciesInfo *)malloc(sizeof(SpeciesInfo));
        printf("on arrive ici?");
        strcpy(outInfo[it].name, name_species);
        strcpy(outInfo[it].id, id_species);
        //récupération du rang
        char *ranked = getRank(line);
        strcpy(outInfo[it].rank, ranked);
        free(ranked);

        strcpy(outInfo[it].id_parent, id_parent_species);
        strcpy(outInfo[it].parent, name_species1);
        printf("value is %s\n", outInfo[it].parent);
        it++;
           

        line = strtok(NULL, "\n");
    }
    //free(outInfo);
    return outInfo;
}

int main(int argc, char **argv)
{
    char *buffer = makebuffer(FICHIER);
    SpeciesInfo *test = fillStructure(buffer);
    free(test);
    free(buffer);
    printf("on sort ");
}
