#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parameters.h"
// #include "blastInfo.h"
// #include "lineage.h"

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
    return strdup(rank_species);
}

/// @brief
/// @param id_parent_species
/// @param buffer
/// @return parent
char *getParentName(char *id_parent_species, char *buffer)
{
    char *line = strtok(strdup(buffer), "\n");
    char *parent = (char *)malloc(sizeof(char) * MIN_SIZE);
    while (line != NULL)
    {
        char id_species[MIN_SIZE], name_species[MIN_SIZE];

        sscanf(line, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species, name_species);
        if (strcmp(id_parent_species, id_species) == 0)
        {
            strcpy(parent, name_species);
            break;
        }
        line = strtok(NULL, "\n");
    }
    return parent;
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
            strcat(lignee, ",\n\t\t\t\t\t\"name\":\"");
            strcat(lignee, name_species);
            strcat(lignee, "\n\t\t\t\t },\n");
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
            if (strcmp(id_parent_species, "") == 0)
            {
                char lignee2[MAX_SIZE];
                strcat(lignee2, "\t\t\t\t {\n\t\t\t\t\t\"taxid\":\"1\"");
                strcat(lignee2, ",\n\t\t\t\t\t\"name\":\"root\"\n\t\t\t\t },\n");
                strcat(lignee2, lineage);
                strcpy(lineage, lignee2);
                return lineage;
            }
            else
            {
                // recursivity
                getLineage(buffer, id_parent_species, lineage);
            }
        }
        line = strtok(NULL, "\n");
    }

    return lineage;
}

char *readTaxoFile(char *buffer, char *species)
{
    char *line = strtok(strdup(buffer), "\n");
    char id_species[MIN_SIZE], name_species[MIN_SIZE], id_parent_species[MIN_SIZE];
    char *lineage = (char *)malloc(4096 * sizeof(char));
    while (line != NULL)
    {
        sscanf(line, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species, name_species);
        if (strcmp(name_species, species) == 0)
        {

            strcat(lineage, "\t\t\t\t {\n\t\t\t\t\t\"taxid\":\"");
            strcat(lineage, id_species);
            strcat(lineage, ",\n\t\t\t\t\t\"name\":\"");
            strcat(lineage, name_species);
            strcat(lineage, "\n\t\t\t\t }");

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

SpeciesInfo *fillStructure(char *buffer)
{
    SpeciesInfo *root = NULL;
    SpeciesInfo *current = NULL;
    char *line = strtok(strdup(buffer), "\n");
    while (line != NULL)
    {
        SpeciesInfo *info = (SpeciesInfo *)malloc(sizeof(SpeciesInfo));
        char id_species[MIN_SIZE], name_species[MIN_SIZE], id_parent_species[MIN_SIZE];
        sscanf(line, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species, name_species);
        strcpy(info->name, name_species);
        strcpy(info->id, id_species);
        char *ranked = getRank(line);
        strcpy(info->rank, ranked);
        free(ranked);

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
        // parent
        char *line2 = strtok(strdup(buffer), "\n");
        char *parent = (char *)malloc(sizeof(char) * MIN_SIZE);
        while (line2 != NULL)
        {
            char id_species[MIN_SIZE], name_species[MIN_SIZE];
            sscanf(line2, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species, name_species);
            if (strcmp(id_parent_species, id_species) == 0)
            {
                strcpy(parent, name_species);
                break;
            }
            line2 = strtok(NULL, "\n");
        }
        strcpy(info->parent, parent);
        free(parent);

        char *lineageResult = readTaxoFile(buffer, name_species);
        strcpy(info->lineage, lineageResult);
        free(lineageResult);

        if (root == NULL)
        {
            printf("on entre ici ?");
            root = info;
        }
        else
        {
            current->next = info;
            info->previous = current;
        }
        current = info;
        line = strtok(NULL, "\n");
    }
    return root;
}

int main(int argc, char **argv)
{

    char *buffer = makebuffer(FICHIER);
    SpeciesInfo *current = fillStructure(buffer);
    while (current != NULL)
    {
        printf("%s %s %s %s %s\n", current->name, current->id, current->rank, current->parent, current->lineage);
        current = current->next;
    }
    free(current);
    free(buffer);

    return 0;
}


void fillStructure1(const char *buffer, const char *species, SpeciesInfo *info) {
    char *dup = strdup(buffer);
    char *line = strtok(dup, "\n");
    while (line != NULL) {
        char id_species[MIN_SIZE], name_species[MIN_SIZE], id_parent_species[MIN_SIZE];
        if (sscanf(line, "%[^\t]\t%*[^\t]\t%[^\t]", id_species, name_species) == 2 && strcmp(species, name_species) == 0) {
            strcpy(info->id, id_species);
            strcpy(info->name, name_species);
            char *ranked = getRank(line);
            strcpy(info->rank, ranked);
            free(ranked);

            int iteration = 0;
            int len = strlen(line);
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
            char *dup2 = strdup(buffer);
            char *line2 = strtok(dup2, "\n");
            char *parent = (char *)malloc(MIN_SIZE);
            while (line2 != NULL) {
                char id_species[MIN_SIZE], name_species[MIN_SIZE];
                if (sscanf(line2, "%[^\t]\t%*[^\t]\t%[^\t]", id_species, name_species) == 2 && strcmp(id_parent_species, id_species) == 0) {
                    strcpy(parent, name_species);
                    break;
                }
                line2 = strtok(NULL, "\n");
            }
            strcpy(info->parent, parent);
            free(parent);
            char *lineageResult = readTaxoFile(buffer, name_species);
            strcpy(info->lineage, lineageResult);
            free(lineageResult);
            break;
        }
        line = strtok(NULL, "\n");
    }
    free(dup);
}
