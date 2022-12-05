#ifndef _TYPE_
#define _TYPE_

#define BUFSIZE 8192
#include <stdio.h>

char buf[BUFSIZE];
char content[BUFSIZE];
int t_from, t_to, query_length;
char *name_hit;
FILE *output;

struct
{
    char query[128];
    char access;
} state;

#endif