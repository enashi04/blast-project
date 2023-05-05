#ifndef LINEAGE_H
#define LINEAGE_H
#include "parameters.h"
#include "hashmap.h"

char *makebuffer(char *filename);
char *getLineage(SpeciesInfo *speciesInfo, char lineage[MAX_SIZE], int id);
char *createLineage(SpeciesInfo *speciesInfo,char *id_parent, Hashmap *hashmap);
char *getParent(SpeciesInfo *speciesInfo, int species_id);

#endif
