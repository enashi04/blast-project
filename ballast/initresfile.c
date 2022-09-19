/*** initresfile *************************************************/
/*                                                               */
/*   Starts reading BLAST result file and stores Header          */
/*   Returns length of query sequence (i.e. of profile)          */
/*                                                               */
/*****************************************************************/
/* BlastHeader *blhd    : BLAST file Header structure            */
/* FILE        *file    : BLAST result file handler              */
/* char        *line    : currently read line                    */
/*                                                               */
/* int     length       : length of query sequence               */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"


int initresfile (FILE *file, char *line, BlastHeader *blhd)
{
int length;
char *str;
char *ptrstr;

fgets (line,256,file);

blhd->prog = NULL;

/*****************************************************************/
/**** Read the BLAST file header *********************************/

while (strstr(line,"Database:") == NULL)
	{
        /*********************************************************/
        /*** Gets BLAST Program name and version *****************/

	if ((strstr(line,"BLAST") != NULL) && (blhd->prog == NULL))
		{
		blhd->prog = (char *) malloc (strlen(line) +1);
		strcpy (blhd->prog, line);
	
		//fprintf(stdout,"le programme Blast est : %s\n", blhd->prog);
		}
        /*********************************************************/
		


        /*********************************************************/
        /*** Gets Query sequence length **************************/

	if (strstr(line,"Query=") != NULL)
		{
		blhd->query = (char *) malloc (strlen(line) +1);
		strcpy (blhd->query, line);
		//fprintf(stdout, "La query est : %s\n ", blhd->query);
		while (strstr(line,"letters)") == NULL)
			{
			fgets (line,256,file);
			blhd->query = (char *) realloc (blhd->query, strlen(blhd->query ) + strlen(line)+1);
			strcat (blhd->query,line);
			//fprintf(stdout, "Bon la query 2e partie est %s\n", blhd->query);
			}
		ptrstr=(char *) (strchr(line,'(')+1);
		str = ptrstr;
		while (*ptrstr != ')')
			{
			if (*ptrstr != ',') {*str = *ptrstr; str++;}
			ptrstr++;
			
			} 
		str=(char *) (strchr(line,'(')+1);
		//fprintf(stdout,"La valeur de str est %s\n",str);
		sscanf (str,"%d",&length);
		//fprintf(stdout, "La longueur est de : %d", length);
		}
        /*********************************************************/
		
	if (!fgets (line,256,file))
		{
		line = NULL;
		return (0);
		}
	}

/*****************************************************************/
/**** Gets the Database name *************************************/

blhd->dbase = (char *) malloc (strlen(line) +1);
strcpy (blhd->dbase, line);
fgets (line,256,file);
blhd->dbase = (char *) realloc (blhd->dbase,strlen(blhd->dbase) + strlen(line)+1);
strcat (blhd->dbase, line); 
//fprintf(stdout, "La db est : %s \n", blhd->dbase);
/*****************************************************************/


/*****************************************************************/
/**** Jump to first BLAST alignment for further reading **********/

while (strncmp (line,"Sequences producing",19) != 0)
	{
	if (!fgets (line,256,file))
		{
		//fprintf(stdout, "Ici on jump au premier alignement askip");
		line = NULL;
		return (0);
		}
	}

if (!fgets (line,256,file)) {line = NULL;return (0);}


while (*line != '>')
	{
	if (!fgets (line,256,file))
		{
		line = NULL;
		return (0);
		}
	}
	
return (length);
}	
/*****************************************************************/
