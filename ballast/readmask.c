/*** readmask   **************************************************/
/*                                                               */
/*   Read a mask from mask file                                  */
/*   returns a pointer to the mask                               */
/*                                                               */
/*****************************************************************/
/* char   *filename     : mask file name                         */
/* char  **mask         : pointer to the mask                    */
/*                                                               */
/* char   line[1024]    : last line read                         */
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readmask (char *filename, char **mask)
{
char line [1024];
FILE *file;

*mask = NULL;

file = fopen (filename, "r");

while (fgets (line,1024,file)!= NULL)
	{
	if (line [strlen(line)-1] == '\n') line [strlen(line)-1] ='\0';
	if (*mask != NULL) 
		{
		*mask = realloc (*mask, strlen (*mask) + strlen (line) + 1 );
		}
	else
		{
		*mask = malloc (strlen (line) + 1 );
		**mask = '\0';
		}
	strcat (*mask,line);
	}

/*
printf ("//%s//\n",*mask);
*/
return (*mask);
}

/*****************************************************************/
