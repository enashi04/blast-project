#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char *buffer ;

char *makebuffer()
{
    FILE *file=fopen("taxonomy.reduced", "r");

    long fileSize;

    if (!file)
    {
        printf("Unable to open file");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    buffer = (char *)malloc((fileSize + 1) * sizeof(char));
    fread(buffer, sizeof(char), fileSize, file);

    buffer[fileSize] = '\0';
    return buffer;
};

char *retrieveParent(char *speciesID, char *lineage)
{
    char *line = strtok(strdup(buffer), "\n");
    char taxID[128], name[128], rank[128], parentID[128];
    char *lignee = (char *)malloc(2058);
    // on revient au début du fichier
    while (line != NULL)
    {
        sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);
        if (strcmp(speciesID, taxID) == 0)
        {
            strcat(lignee, name);
            strcat(lignee, "/");
            strcat(lignee, lineage);
            strcpy(lineage, lignee);
            retrieveParent(parentID, lineage);
        }
        line = strtok(NULL, "\n");
    }
    return lineage;
};

char *getParentName(char *speciesID){
    char *parentName=speciesID;
    char *line = strtok(strdup(buffer), "\n");
    char taxID[128], name[128], rank[128], parentID[128];
    // on revient au début du fichier
    while (line != NULL)
    {
        sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);

        if (strcmp(speciesID, taxID) == 0)
        {
            strcpy(parentName, name);
            break;
        }
        line = strtok(NULL, "\n");
    }
    return parentName;
};

char *readTaxo(char *species_name)
{
    char *line = strtok(strdup(buffer), "\n");

    char taxID[128], name[128], rank[128], parentID[128];
    char *lineage = (char *)malloc(1024);
    char *finalResult = (char *)malloc(1024);

    while (line != NULL)
    {
        sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);
        if (strcmp(name, species_name) == 0)
        {
            if (strcmp(rank, "superkingdom") == 0)
            {
                finalResult = name;
                break;
            }
            else
            {
                strcpy(lineage, name);
                finalResult = retrieveParent(parentID, lineage);
            }
        }
        line = strtok(NULL, "\n");
    }
    return finalResult;
};


