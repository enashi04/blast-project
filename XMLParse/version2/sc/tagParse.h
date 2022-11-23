#ifndef _TAGPARSE_
#define _TAGPARSE_
#include <expat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFSIZE 8192

extern char buf[BUFSIZE];
extern char content[BUFSIZE];
extern int t_from, t_to, query_length;
extern char *name_hit;
extern FILE *output;

extern struct
{
    char query[128];
    char access;
} state;

/** @brief Open the tag for the Bronze Mode
    @param data
    @param name
    @param attrs
*/
void bronze_tag_start(void *data, const char *name, const char **attrs);

/** @brief Open the tag for the Silver Mode
    @param data
    @param name
    @param attrs
*/
void silver_tag_start(void *data, const char *name, const char **attrs);

/** @brief Open the tag for the Gold Mode
    @param data
    @param name
    @param attrs
*/
void gold_tag_start(void *data, const char *name, const char **attrs);

/** @brief Closing the tag
    @param data
    @param name
*/
void tag_end(void *data, const char *name);

/** @brief Read the tag content
    @param data
    @param text
    @param len
*/
void tag_value(void *data, const char *text, int len);

/** @brief display errors and close file f
    @param f
    @param parser
*/
void test_error(FILE *f, XML_Parser parser);
#endif