#ifndef LINEAGE_H
#define LINEAGE_H

char *makebuffer(char *filename);
char *getLineage(char *buffer,char *speciesID, char *lineage);
char *getParentName(char *buffer,char *speciesID);
char *readTaxoFile(char *buffer,char *species_name);

#endif
