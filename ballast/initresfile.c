#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include <ctype.h>

char *substr(const char *src, int m, int n);

int initresfile(FILE *file, char *line, BlastHeader *blhd)
{
    int length;

    fgets(line, 256, file);

    blhd->prog = NULL;

    while (strstr(line, "Length=") == NULL)
    {
        blastVersion(blhd, line);
        queryName(blhd, line, file);
        databaseName(blhd, line, file);

        if (!fgets(line, 256, file))
        {
            line = NULL;
            return (0);
        }
    }

    length = getLength(line, length);
    jumpAlignment(line, file);

    if (!fgets(line, 256, file))
    {
        line = NULL;
        return (0);
    }

    while (*line != '>')
    {
        if (!fgets(line, 256, file))
        {
            line = NULL;
            return (0);
        }
    }
    return length;
}


/*
 Substring
*/
char *substr(const char *src, int m, int n)
{
    // get the length of the destination string
    int len = n - m;

    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char *)malloc(sizeof(char) * (len + 1));

    // start with m'th char and copy `len` chars into the destination
    strncpy(dest, (src + m), len);

    // return the destination string
    return dest;
}

/*
  Gets BLAST Program name and version 
 */
void blastVersion(BlastHeader *blhd, char *line)
{

    if ((strstr(line, "BLASTP") != NULL) && (blhd->prog == NULL))
    {
        blhd->prog = (char *)malloc(strlen(line) + 1);
        strcpy(blhd->prog, line);
    }
}

/*
  Gets Query sequence length
 */
void queryName(BlastHeader *blhd, char *line, FILE *file)
{

    if (strstr(line, "Query=") != NULL && (blhd->query == NULL))
    {
        blhd->query = (char *)malloc(strlen(line) + 1);
        strcpy(blhd->query, line);
        fgets(line, 256, file);
        blhd->query = (char *)realloc(blhd->query, strlen(blhd->query) + strlen(line) + 1);
        strcat(blhd->query, line);
        // fprintf(stdout,"la query est : %s\n",blhd->query );
    }
}

/*
  Gets the Database name 
*/
void databaseName(BlastHeader *blhd, char *line, FILE *file)
{
    if (strstr(line, "Database:") != NULL && (blhd->dbase == NULL))
    {
        blhd->dbase = (char *)malloc(strlen(line) + 1);
        strcpy(blhd->dbase, line);

        fgets(line, 256, file);
        blhd->dbase = (char *)realloc(blhd->dbase, strlen(blhd->dbase) + strlen(line) + 1);
        strcat(blhd->dbase, line);
    }
}

/*
 Jump to first BLAST alignment for further reading 
*/
void jumpAlignment(char *line, FILE *file)
{
    while (strncmp(line, "Sequences producing", 19) != 0)
    {
        if (!fgets(line, 256, file))
        {
            line = NULL;
            return (0);
        }
    }
}
/*
 Get Length
*/
int getLength(char *line, int length)
{
    if (strstr(line, "Length=") != NULL)
    {
        char *longueur;
        longueur = (char *)malloc(strlen(line) + 1);
        strcpy(longueur, line);

        char *query;
        query = longueur;
        for (int i = 0; i < strlen(longueur) - 1; i++)
        {
            if (isdigit(query[i]) != 0)
            {
                char *newquery = substr(query, i, strlen(longueur));
                length = atoi(newquery);
                break;
            }
        }
    }
}
/**************************************************************************/