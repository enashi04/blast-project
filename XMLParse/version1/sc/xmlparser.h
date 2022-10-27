#include "lib/libxml/parser.h"
#include "lib/libxml/tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hit_enter(xmlNode *node, FILE *fp);
void query_Def(xmlNode *node, FILE *fp);
void query_Length(xmlNode *node, FILE *fp);
void hit_ID(xmlNode *node, FILE *fp, int mode);
void getHSP(xmlNode *node, const char *name, FILE *fp);
void HSP_Enter(xmlNode *node, FILE *fp, int mode);
void hit_Iteration(xmlNode *node, FILE *fp, int mode);
void blastOutPut_iteration(xmlDoc *fichier, FILE *fp, int mode);
void selectedParameters(FILE *fp,int mode);
