#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "fillStructure.h"
#include "blastInfo.h"
#include "parameters.h"



SpeciesInfo *fillStructure(char *buffer)
{
    SpeciesInfo *outInfo = (SpeciesInfo *)malloc(SPECIES_SIZE * sizeof(SpeciesInfo));

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