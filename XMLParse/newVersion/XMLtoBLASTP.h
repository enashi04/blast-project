#ifndef XMLTOBLAST_H
#define XMLTOBLAST_H
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parameters.h"
#include "lib/libxml/parser.h"
#include "lib/libxml/tree.h"


char *replaceWord(const char *s, const char *oldW, const char *newW);

char *blast_reference(xmlNode *node);

void convertToBlastP(xmlDoc *xmlfile, xmlNode *child, char *blastInfo, char *database);

char *getInfoBlast(xmlNode *node, char *database);

void query_Def(xmlNode *node, FILE *output);

void query_Length(xmlNode *node, FILE *output);

void statNode(xmlNode *node,FILE  *output, char *database);

void hitNode(xmlNode *node, FILE *output);

void hspNode(xmlNode *node, FILE *output);

void blasting(xmlNode *node, FILE *output);

void iterationNode(xmlNode *node, FILE *output, char *database);

#endif

// /*****************************************QUERY*****************************************/
// query_Def(node, output); //la query def c'est autre part
// /*****************************************LONGUEUR DE LA QUERY*****************************************/
// query_Length(node, output);
/*****************************************NOEUD CONTENANT LES ITERATIONS*****************************************/