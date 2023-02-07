#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parameters.h"
#include "blastInfo.h"

SpeciesInfo *fillStructure(char *buffer){
    SpeciesInfo *infos_espece = NULL;
    SpeciesInfo *head = NULL;
    SpeciesInfo *tail = NULL;
    char *line = strtok(strdup(buffer), "\n");
    while (line != NULL) {
        SpeciesInfo *info_espece = (SpeciesInfo *)malloc(sizeof(SpeciesInfo));
        info_espece->next = NULL;
        info_espece->previous = NULL;
        char id_species[MIN_SIZE] = {0};
        char name_species[MIN_SIZE] = {0};
        char rank_species[MIN_SIZE] = {0};
        char id_parent_species[MIN_SIZE] = {0};
        sscanf(line, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species, name_species);
        info_espece->name = strdup(name_species);
        info_espece->id = strdup(id_species);
        info_espece->rank = strdup(getRank(line));

        //récupérer le parent 
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
        info_espece->parent = strdup(getParentName(id_parent_species, buffer));

        if (head == NULL) {
            head = info_espece;
            tail = info_espece;
        } else {
            tail->next = info_espece;
            info_espece->previous = tail;
            tail = info_espece;
        }
        line = strtok(NULL, "\n");
    }
    return head;
}

//manque plus que lineage
