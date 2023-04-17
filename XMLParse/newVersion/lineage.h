#ifndef LINEAGE_H
#define LINEAGE_H
#include "parameters.h"
#include "hashmap.h"

char *makebuffer(char *filename);
char *getLineage(SpeciesInfo *speciesInfo, int id, char lineage[MAX_SIZE], int parentTarget);
char *createLineage(SpeciesInfo *speciesInfo,char *id_parent, Hashmap *hashmap);

#endif
