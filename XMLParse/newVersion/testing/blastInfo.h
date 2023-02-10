#ifndef BLASTINFO_H
#define BLASTINFO_H

#include "lib/libxml/parser.h"
#include "lib/libxml/tree.h"
#include "retrieveInfo.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parameters.h"
#include "lineage.h"

void getBlastVersion(xmlNode *node);
void getBlastDB(xmlNode *node);
void getQueryDef(xmlNode *node, char species[MIN_SIZE]);
//char *retrieveDef(xmlNode *node);
// int getQueryLength(xmlNode *node);
char *getQuerySpeciesName(char *species);

char *getRank(char *line);

void displayQuerySpecies(char *species, char *buffer);
char *getParentName(char *id_parent_species, char *buffer);

#endif