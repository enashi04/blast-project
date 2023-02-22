#ifndef RETRIEVEINFO_H
#define RETRIEVEINFO_H

#include "lib/libxml/parser.h"
#include "lib/libxml/tree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parameters.h"

extern FILE *output;

void blastOutPut_iteration(xmlDoc *fichier, char *mode, char *buffer);
void hit_enter(xmlNode *node);

char *getHitAccession(xmlNode *node);
void getHSP(xmlNode *node, const char *name, char *label, int query_length);
void node_HSP(xmlNode *node,  char *mode, char *hit_id, char *species, int query_length, SpeciesInfo *speciesInfo);
void node_Iteration(xmlNode *node, char *mode, char *buffer, int query_length);
char *getSpecies(xmlNode *node);

#endif