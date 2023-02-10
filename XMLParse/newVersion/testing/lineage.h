#ifndef LINEAGE_H
#define LINEAGE_H
#include "parameters.h"
char *makebuffer(char *filename);
char *getParent(SpeciesInfo *speciesInfo, char *id);
char *getLineage(SpeciesInfo *speciesInfo, char *id, char lineage[MAX_SIZE], char *parentTarget);
char *createLineage(SpeciesInfo *speciesInfo,char *id_parent);
#endif
