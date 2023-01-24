#include "lib/libxml/parser.h"
#include "lib/libxml/tree.h"
#include "hashMap.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

 FILE *output;
int query_length, t_from, t_to;
 HashTable *table;


void hit_enter(xmlNode *node);
void query_Def(xmlNode *node);
void query_Length(xmlNode *node);
char *hit_ID(xmlNode *node,char *mode);
void getHSP(xmlNode *node, const char *name, char *label);
void HSP_Enter(xmlNode *node,  char *mode, char *hit_id, char *species);
void hit_Iteration(xmlNode *node, char *mode);
char *getSpecies(xmlNode *node);
void blastOutPut_iteration(xmlDoc *fichier, char *mode, FILE *file);
