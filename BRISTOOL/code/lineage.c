#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "lineage.h"
#include "blastInfo.h"
#include "fillStructure.h"


/**************************************************************************************************************/
/*                           getLineage : get the lineage of the species                                      */
/** Parametre : speciesInfo : structure of informations of species                                            */
/**             id : taxid of the species                                                                     */
/**             lineage : stock the lineage of the species (return value)                                     */
/**             parentTarget : taxId of the parent                                                            */
/**************************************************************************************************************/
char *getLineage(SpeciesInfo *speciesInfo, char lineage[MAX_SIZE], int id)
{
    //initialize another string to stock the lineage
    char lignee[MAXI_SIZE];
    //stock the taxid and the name of the species
    strcpy(lignee, "{\"taxid\":");
    char str_id[MIN_SIZE];
    //convert the string to int
    sprintf(str_id, "%d", id); 
    // concatenate the string
    strcat(lignee, str_id);
    strcat(lignee, ",\"name\":\"");
    strcat(lignee, speciesInfo[id].name);
    strcat(lignee, "\"},");
    strcat(lignee, lineage);
    strcpy(lineage, lignee);
    //if the parent is the root, we add the information of the root in lineage
    if (speciesInfo[id].parentid == 0)
    {
        // initialize another string to stock the lineage
        char lignee2[MAXI_SIZE] ="\",\"lineage\":[{\"taxid\":1";
        strcat(lignee2, ",\"name\":\"root\"},");
        strcat(lignee2, lineage);
        strcpy(lineage, lignee2);
    }
    else
    {
        // call the function recursively to get the lineage of the parent
        lineage = getLineage(speciesInfo, lineage, speciesInfo[id].parentid);
    }
    //return the lineage
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
    //initialize the lineage
    char *lineage = malloc(sizeof(char) * MAXI_SIZE);
    //get the index of the species in the hashmap
    int index = get(hashmap, species);
    //we add the first information in lineage (taxid and name) of the species
    strcpy(lineage, "{\"taxid\":");
    char str_i[MIN_SIZE];
    sprintf(str_i, "%d", index);
    strcat(lineage, str_i);
    strcat(lineage, ",\"name\":\"");
    strcat(lineage, species); 
    strcat(lineage, "\"}]");
    //get the taxid of the parent
    int parentTaxId = speciesInfo[index].parentid;
    // add the information of the parent in lineage by calling a recursive function
    char *returnLineage = getLineage(speciesInfo, lineage, parentTaxId);
    // add the information of the parent before the lineage
    strcpy(lineage, "\"parent\":\"");
    strcat(lineage, speciesInfo[parentTaxId].name);
    strcat(lineage, returnLineage);
    // int len = strlen(lineage);
    // lineage[len-1] = '\0';
    // return the lineage and the parent.
    return strdup(lineage);
}

/*
char *getLineage(SpeciesInfo *speciesInfo, char lineage[MAX_SIZE], int id)
{
    //initialize another string to stock the lineage
    char lignee[MAXI_SIZE];
    //stock the taxid and the name of the species
    strcpy(lignee, "\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":");
    char str_id[MIN_SIZE];
    //convert the string to int
    sprintf(str_id, "%d", id); 
    // concatenate the string
    strcat(lignee, str_id);
    strcat(lignee, ",\n\t\t\t\t\t\t\"name\":\"");
    strcat(lignee, speciesInfo[id].name);
    strcat(lignee, "\"\n\t\t\t\t\t },\n");
    strcat(lignee, lineage);
    strcpy(lineage, lignee);
    //if the parent is the root, we add the information of the root in lineage
    if (speciesInfo[id].parentid == 0)
    {
        // initialize another string to stock the lineage
        char lignee2[MAXI_SIZE] ="\",\n\t\t\t\t\t\"lineage\":[\n\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":1";
        strcat(lignee2, ",\n\t\t\t\t\t\t\"name\":\"root\"\n\t\t\t\t\t },\n");
        strcat(lignee2, lineage);
        strcpy(lineage, lignee2);
    }
    else
    {
        // call the function recursively to get the lineage of the parent
        lineage = getLineage(speciesInfo, lineage, speciesInfo[id].parentid);
    }
    //return the lineage
    return strdup(lineage);
}
char *createLineage(SpeciesInfo *speciesInfo, char *species, Hashmap *hashmap)//faire appel à la hashmap ici
{
    //initialize the lineage
    char *lineage = malloc(sizeof(char) * MAXI_SIZE);
    //get the index of the species in the hashmap
    int index = get(hashmap, species);
    //we add the first information in lineage (taxid and name) of the species
    strcpy(lineage, "\t\t\t\t\t {\n\t\t\t\t\t\t\"taxid\":");
    char str_i[MIN_SIZE];
    sprintf(str_i, "%d", index);
    strcat(lineage, str_i);
    strcat(lineage, ",\n\t\t\t\t\t\t\"name\":\"");
    strcat(lineage, species); 
    strcat(lineage, "\"\n\t\t\t\t\t }\n\t\t\t\t\t]");
    //get the taxid of the parent
    int parentTaxId = speciesInfo[index].parentid;
    // add the information of the parent in lineage by calling a recursive function
    char *returnLineage = getLineage(speciesInfo, lineage, parentTaxId);
    // add the information of the parent before the lineage
    strcpy(lineage, "\t\t\t\t\t\"parent\":\"");
    strcat(lineage, speciesInfo[parentTaxId].name);
    strcat(lineage, returnLineage);
    // return the lineage and the parent.
    return strdup(lineage);
}*/
