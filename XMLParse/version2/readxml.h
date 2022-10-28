#include <stdio.h>
#include <string.h>
#include <expat.h>

#define BUFSIZE 8192

void tag_start(void *data, const char *name, const char **attrs);
void tag_end(void *data, const char *name);
void tag_value(void *data, const char *text, int len);

char buf[BUFSIZE];
char content[BUFSIZE];

struct {
	char query[128];
	char access;
} state;

