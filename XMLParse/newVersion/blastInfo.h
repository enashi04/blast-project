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
void getQueryDef(xmlNode *node);
char *retrieveDef(xmlNode *node);
void getQueryLength(xmlNode *node, char *speciesName,int query_length);
char *getQuerySpeciesName(char *species);
char *getQuerySpeciesId(char *species, char *buffer);
void displayQuerySpecies(char *species, char *buffer);
#endif