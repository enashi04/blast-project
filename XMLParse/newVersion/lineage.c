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
    // Open the taxonomy file
    FILE *file = fopen(filename, "r");
    // check of the file existence
    if (!file)
    {
        printf("Unable to open file taxonomy.dat");
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
    // close the file
    fclose(file);
    return buffer;
}
/**************************************************************************************************************/
/*                           getLineage : get the lineage of the species                                      */
/** Parametre : speciesInfo : structure of informations of species                                            */
/**             id : taxid of the species                                                                     */
/**             lineage : stock the lineage of the species (return value)                                     */
/**             parentTarget : taxId of the parent                                                            */
/**************************************************************************************************************/
char *getLineage(SpeciesInfo *speciesInfo, char lineage[MAX_SIZE], int id)
{
    char lignee[MAXI_SIZE];
    strcpy(lignee, "\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":\"");

    char str_id[MIN_SIZE];
    sprintf(str_id, "%d", id); //convertiru en int

    strcat(lignee, str_id);
    strcat(lignee, "\",\n\t\t\t\t\t\t\"name\":\"");
    strcat(lignee, speciesInfo[id].name);
    strcat(lignee, "\"\n\t\t\t\t\t },\n");
    strcat(lignee, lineage);
    strcpy(lineage, lignee);

    if (speciesInfo[id].parentid == 0)
    {
        char lignee2[MAXI_SIZE] ="\",\n\t\t\t\t\t\"lineage\":[\n\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":\"1\"";
        strcat(lignee2, ",\n\t\t\t\t\t\t\"name\":\"root\"\n\t\t\t\t\t },\n");
        strcat(lignee2, lineage);
        strcpy(lineage, lignee2);
    }
    else
    {
        lineage = getLineage(speciesInfo, lineage, speciesInfo[id].parentid);
    }
    return strdup(lineage);
}
/**************************************************************************************************************/
/*                           createLineage:create the lineage of the species                                  */
/** Parametre : speciesInfo : structure of informations of species                                            */
/**             species : name of the species                                                                 */
/**             hashmap : hashmap containing informations about the taxid and name of the species             */
/** Return : lineage                                                                                          */
/**************************************************************************************************************/
char *createLineage(SpeciesInfo *speciesInfo, char *species, Hashmap *hashmap)//faire appel à la hashmap ici
{
    char *lineage = malloc(sizeof(char) * MAXI_SIZE);
    int index = get(hashmap, species);
    strcpy(lineage, "\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":\"");
    char str_i[MIN_SIZE];
    sprintf(str_i, "%d", index);
    strcat(lineage, str_i);
    strcat(lineage, "\",\n\t\t\t\t\t\t\"name\":\"");
    strcat(lineage, species); //on peut utiliser speciesInfo[index].name 
    strcat(lineage, "\"\n\t\t\t\t\t }\n\t\t\t\t\t]");
    char *returnLineage = getLineage(speciesInfo, lineage, speciesInfo[index].parentid);
    strcpy(lineage, "\t\t\t\t\t\"parent\":\"");
    strcat(lineage, speciesInfo[speciesInfo[index].parentid].name);
    strcat(lineage, returnLineage);
    return strdup(lineage);
}
