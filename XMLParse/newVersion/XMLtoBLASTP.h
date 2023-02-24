#ifndef XMLTOBLAST_H
#define XMLTOBLAST_H
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parameters.h"
#include "lib/libxml/parser.h"
#include "lib/libxml/tree.h"

void blastInfo(xmlDoc *xmlfile, FILE *output, xmlNode *child);

char *replaceWord(const char *s, const char *oldW, const char *newW);

void blast_version(xmlNode *node, FILE *output);

void blast_reference(xmlNode *node, FILE *output);

void blast_db(xmlNode *node, FILE *output, char *database);

void query_Def(xmlNode *node, FILE *output);

void query_Length(xmlNode *node, FILE *output);

void statNode(xmlNode *node,FILE  *output, char *database);

void hitNode(xmlNode *node, FILE *output);

void hspNode(xmlNode *node, FILE *output);

void blasting(xmlNode *node, FILE *output);

void iterationNode(xmlNode *node, FILE *output, char *database);

#endif

