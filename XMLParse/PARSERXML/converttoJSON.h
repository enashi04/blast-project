//Conversion en JSON
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <expat.h>
#include "readTaxonomy.h"


 char buf[BUFSIZE];
 char content[BUFSIZE];
 int t_from, t_to, query_length;
 FILE *output;
 HashTable *table;

 struct
{
    char query[128];
    char access;
} state;



char *getSpecies(char *content);
void bronze_tag_start(void *data, const char *name, const char **attrs);
void silver_tag_start(void *data, const char *name, const char **attrs);
void gold_tag_start(void *data, const char *name, const char **attrs);
void tag_end(void *data, const char *name);
void tag_value(void *data, const char *text, int len); //ajouter la hashmap? ici ... 
void test_error(FILE *f, XML_Parser parser);
