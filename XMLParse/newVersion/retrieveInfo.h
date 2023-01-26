#ifndef RETRIEVEINFO_H
#define RETRIEVEINFO_H

#include "lib/libxml/parser.h"
#include "lib/libxml/tree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parameters.h"

FILE *output;

void hit_enter(xmlNode *node);
// void getBlastVersion(xmlNode *node);
// void getBlastDB(xmlNode *node);
// void getQueryDef(xmlNode *node, char *speciesName);
// void getQueryLength(xmlNode *node,char *speciesName);

char *getHitAccession(xmlNode *node,char *mode);
void getHSP(xmlNode *node, const char *name, char *label);
void node_HSP(xmlNode *node,  char *mode, char *hit_id, char *species, char *buffer,SpeciesInfo *species_info);
void node_Iteration(xmlNode *node, char *mode, char *buffer);
char *getSpecies(xmlNode *node);

void blastOutPut_iteration(xmlDoc *fichier, char *mode, char *buffer);

#endif