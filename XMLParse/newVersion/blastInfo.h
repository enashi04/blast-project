#ifndef BLASTINFO_H
#define BLASTINFO_H

#include "lib/libxml/parser.h"
#include "lib/libxml/tree.h"
#include "retrieveInfo.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parameters.h"

void blastVersion(xmlNode *node);
void blastDB(xmlNode *node);
// void getQueryInfo(xmlNode *child, char species[MIN_SIZE], int query_length, char* iteration_num);
char *getQuerySpeciesName(char *species);
char *getQuerySpeciesID(char *species);
char *getRank(char *line);
void displayQuerySpecies(char *species);

#endif