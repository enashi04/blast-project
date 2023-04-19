#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "fillStructure.h"
#include "blastInfo.h"
#include "parameters.h"


/**************************************************************************************************************/
/*                           fillStructure: fill the Specie Structure                                         */
/** Parametre : buffer : taxonomy.dat                                                                         */
/**************************************************************************************************************/
SpeciesInfo *fillStructure(char *buffer)
{
    SpeciesInfo *outInfo = (SpeciesInfo *)malloc(SPECIES_SIZE * sizeof(SpeciesInfo));

    char *line = strtok(strdup(buffer), "\n");
    line = strtok(NULL, "\n");

    for (int i = 0; i < SPECIES_SIZE && line != NULL; i++)
    {
        //initialize variables for the structure
        char id_species[MIN_SIZE], name_species[MIN_SIZE], id_parent_species[MIN_SIZE];
        //get variables from the line
        sscanf(line, "%[^	]	%*[^	]	%[^	]	%*[^\n]", id_species, name_species);
        //convert the id_species from char to int
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
                    //get the taxid of the parent of the species
                    if (iteration == 9)
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
            //fill the structure
            outInfo[i].name = strdup(name_species);
            outInfo[i].rank = getRank(line);
            outInfo[i].parentid = atoi(id_parent_species);
            //get the next line
            line = strtok(NULL, "\n");
        }
        else
        {
            outInfo[i].name = "\0";
            outInfo[i].rank = "\0";
            outInfo[i].parentid = 0;
        }
    }
    return outInfo;
}