#ifndef RETRIEVEINFO_H
#define RETRIEVEINFO_H

#include "lib/libxml/parser.h"
#include "lib/libxml/tree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parameters.h"
#include "hashmap.h"

FILE *output;

void blastOutPut_iteration(xmlDoc *fichier, char *mode, char *buffer, char tabInfo[13][2][20]);
void hit_enter(xmlNode *node);

char *getHitAccession(xmlNode *node);
char *getHSP(xmlNode *node, const char *name);
char *node_HSP(xmlNode *node,  char *mode, int query_length, SpeciesInfo *speciesInfo, FillSpeciesInfo *fillInfo, Hashmap *hashmap, char tabInfo[13][2][20]);
char *node_Iteration(xmlNode *node, char *mode, SpeciesInfo *speciesInfo, int query_length,FillSpeciesInfo *fillInfo, Hashmap *hashmap,char tabInfo[13][2][20], char *iteration_num);
char *getSpecies(xmlNode *node, char *fragment);

#endif