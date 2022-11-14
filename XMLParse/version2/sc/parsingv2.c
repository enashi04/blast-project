#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>
#include "options.h"
#define BUFSIZE 8192

char buf[BUFSIZE];
char content[BUFSIZE];
FILE *output;
//int mode;


struct
{
    char query[128];
    char access;
} state;


/** @brief Open the tag for the Bronze Mode
    @param data
    @param name
    @param attrs
*/
void bronze_tag_start(void *data, const char *name, const char **attrs)
{
    if (strcmp(name, "Hit_accession") == 0)
    {
        state.access = 5;
    }
    if (strcmp(name, "Hsp_num") == 0)
    {
        state.access = 6;
    }
    if (strcmp(name, "Hsp_score") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_evalue") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_align-len") == 0)
    {
        state.access = 2;
    }
    // if (strcmp(name, "Iteration_query-def") == 0)
    // {
    //     state.access = 3;
    // }
    // if (strcmp(name, "Iteration_query-len") == 0)
    // {
    //     state.access = 4;
    // }
}

/** @brief Open the tag for the Silver Mode
    @param data
    @param name
    @param attrs
*/
void silver_tag_start(void *data, const char *name, const char **attrs)
{

    if (strcmp(name, "Hit_accession") == 0)
    {
        state.access = 5;
    }
    if (strcmp(name, "Hsp_num") == 0)
    {
        state.access = 6;
    }
    if (strcmp(name, "Hsp_score") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_evalue") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_identity") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_positive") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_gaps") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_align-len") == 0)
    {
        state.access = 2;
    }
    // if (strcmp(name, "Iteration_query-def") == 0)
    // {
    //     state.access = 3;
    // }
    // if (strcmp(name, "Iteration_query-len") == 0)
    // {
    //     state.access = 4;
    // }
}
/** @brief Open the tag for the Gold Mode
    @param data
    @param name
    @param attrs
*/
void gold_tag_start(void *data, const char *name, const char **attrs)
{

    if (strcmp(name, "Hit_accession") == 0)
    {
        state.access = 5;
    }
    if (strcmp(name, "Hsp_num") == 0)
    {
        state.access = 6;
    }
    if (strcmp(name, "Hsp_bit-score") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_score") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_evalue") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_query-from") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_query-to") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_hit-from") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_hit-to") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_identity") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_positive") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_gaps") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_align-len") == 0)
    {
        state.access = 2;
    }
}

/** @brief Closing the tag
    @param data
    @param name
*/
void tag_end(void *data, const char *name)
{
}

/** @brief Read the tag content
    @param data
    @param text
    @param len
*/
void tag_value(void *data, const char *text, int len)
{
    strncpy(content, text, len);
    content[len] = '\0';

    if (state.access == 1)
    {
        fprintf(output, "%s,", content);
        strcpy(content, state.query);
        state.access = 0;
    }
    if (state.access == 2)
    {
        fprintf(output, "%s\n", content);
        strcpy(content, state.query);
        state.access = 0;
    }
  
    if (state.access == 5) // pour le nom de l'hit
    {
        fprintf(output, "%s,", content);
        strcpy(content, state.query);
        state.access = 0;
    }
    if (state.access == 6) // pour le numéro du hit
    {
        if (strncmp(content, "1", 1) == 0)
        {
            fprintf(output, "%s,", content);
            strcpy(content, state.query);
            state.access = 0;
        }
        else
        {
            fprintf(output, "-,%s,", content);
            strcpy(content, state.query);
            state.access = 0;
        }
    }
}

/** @brief display errors and close file f
    @param f
    @param parser
*/
void test_error(FILE *f, XML_Parser parser)
{
    int done;
    do
    {
        int len = fread(buf, 1, BUFSIZE, f);

        if (ferror(f))
        {
            fprintf(stderr, "Read error\n");
            break;
        }
        done = feof(f);

        if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR)
        {
            fprintf(stderr, "Error while parsing XML\n");
            break;
        }
    } while (!done);

    XML_ParserFree(parser);
    fclose(f);
}


int main(int argc, char **argv){
    if (argc==1)
    {
        fprintf(stderr,"Usage:\n");
        fprintf(stderr,"\t./parsing -option <argument>\n\n");
        fprintf(stderr, "-h or help to see options\n");
        exit(1);
    }
    char *name=NULL;
    char *outName = NULL;
    int displayConsole = 0;
    getArgs(argc, argv,name, outName, displayConsole);

    // Affichage du résultat en fonction du mode

    //affichage du résultat sur la console

    //affichage d'un exemple CSV
    return 0;
}