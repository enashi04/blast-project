#ifndef LINEAGE_H
#define LINEAGE_H
#include "parameters.h"
char *makebuffer(char *filename);
char *getParentName(char *buffer,char *speciesID);
char *getLineage(SpeciesInfo *speciesInfo, char *id, char lineage[MAX_SIZE]);
char *createLineage(SpeciesInfo *speciesInfo,char *id_parent);
#endif
