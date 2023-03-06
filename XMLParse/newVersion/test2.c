#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parameters.h"

// buffer pour récupérer la taxonomie.
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
        if (line[i] == '	') // change this part cause 2 bumps
        {
            iteration++;
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
    }
    return strdup(rank_species);
}


SpeciesInfo *fillStructure(char *buffer)
{
    SpeciesInfo *outInfo = (SpeciesInfo *)malloc(FILE_SIZE * sizeof(SpeciesInfo));

    char *line = strtok(strdup(buffer), "\n");
    line = strtok(NULL, "\n");

    for (int i = 0; i < SPECIES_SIZE && line != NULL; i++)
    {
        char id_species[MIN_SIZE], name_species[MIN_SIZE], other_name_species[MAXI_SIZE], id_parent_species[MIN_SIZE];
        sscanf(line, "%[^	]	%*[^	]	%[^	]	%*[^\n]", id_species, name_species);
        int taxid = atoi(id_species);
        if (i == taxid)
        {
            int len = strlen(line);
            int iteration = 0;
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
                    else if (iteration == 9)
                    {
                        int k = 0;
                        for (int l = j + 1; l < len; l++)
                        {
                            if (line[l] == '\t')
                            {
                                id_parent_species[k] = '\0';
                                break;
                            }
                            else
                            {
                                id_parent_species[k] = line[l];
                            }
                            k++;
                        }
                        break;
                    }
                }
            }
            outInfo[i].name = strdup(name_species);
            outInfo[i].rank = getRank(line);
            outInfo[i].othername = strdup(other_name_species);
            outInfo[i].parentid = atoi(id_parent_species);
            line = strtok(NULL, "\n");
        }
        else
        {
            outInfo[i].name = "\0";
            outInfo[i].rank = "\0";
            outInfo[i].othername = "\0";
            outInfo[i].parentid = 0;
        }
    }

    return outInfo;
}
char *getLineage(SpeciesInfo *speciesInfo, int id, char lineage[MAX_SIZE], int parentTarget)
{
    char lignee[MAX_SIZE];
    lignee[0] = '\0';

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
        char lignee2[MAXI_SIZE] = "\",\n\t\t\t\t\t\"parent\":\"";
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
    char lineage[MAXI_SIZE];
    for(int i =0; i<SPECIES_SIZE; i++) {
        char *check_string = strstr(speciesInfo[i].othername,species);
        if (strcmp(species, speciesInfo[i].name) == 0)
        { 
            strcpy(lineage, "\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":\"");
            char str_i[MIN_SIZE];
            sprintf(str_i, "%d", i);
            strcat(lineage, str_i);
            strcat(lineage, "\",\n\t\t\t\t\t\t\"name\":\"");
            strcat(lineage, speciesInfo[i].name);
            strcat(lineage, "\"\n\t\t\t\t\t }\n\t\t\t\t\t]\n\t\t\t\t}],");
            printf("id est |%d|\n", speciesInfo[i].parentid);
            printf("lineag is %s\n",  speciesInfo[speciesInfo[i].parentid].name);

            char *returnLineage = getLineage(speciesInfo, speciesInfo[i].parentid, lineage, speciesInfo[i].parentid);
            strcpy(lineage, returnLineage);
            break;
        }
        else if (check_string != NULL)
        {
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

int main(int argc, char **argv)
{
    char *buffer = makebuffer(FICHIER);
    SpeciesInfo *test = fillStructure(buffer);
     char *lineage = createLineage(test,"Mycobacterium paraense");
     printf("la lignée est %s\n", lineage);

}