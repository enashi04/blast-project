#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parameters.h"
#include "fillStructure.h"
#include "blastInfo.h"
#include "lineage.h"

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
