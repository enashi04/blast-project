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
void getHSP(xmlNode *node, const char *name, int query_length);
void node_HSP(xmlNode *node,  char *mode, int query_length, SpeciesInfo *speciesInfo, FillSpeciesInfo *fillInfo, Hashmap *hashmap, char tabInfo[13][2][20]);
void node_Iteration(xmlNode *node, char *mode, SpeciesInfo *speciesInfo, int query_length,FillSpeciesInfo *fillInfo, Hashmap *hashmap,char tabInfo[13][2][20], char *iteration_num);
char *getSpecies(xmlNode *node, char *fragment);

#endif