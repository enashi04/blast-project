/*** Print usage *************************************************/
/*                                                               */
/*   Prints usage of program from a *.hlp file                   */
/*   No return value.                                            */
/*                                                               */
/*****************************************************************/
/* char   *program      : program name (the help file should     */
/*                        have the same name with extension .hlp)*/
/* char   *path         : path where the help file is supposed   */
/*                        to be located                          */
/*                                                               */
/* FILE   *helpfile     : help file handler                      */
/* char   *helpfilename : help file name                         */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printusage (char *program, char *path)
{
FILE *helpfile;
char *helpfilename;
char line[256];


/*****************************************************************/
/*** Determines help file name from program name and path      ***/
/*****************************************************************/

helpfilename = (char *) malloc (strlen (path) + strlen (program) + 6);

sprintf (helpfilename,"%s/%s.hlp",path,program);
/*****************************************************************/


/*****************************************************************/
/*** Prints usage to stdin                                     ***/
/*****************************************************************/

if (helpfile = fopen (helpfilename,"r"))
	{
	while ( strncmp(fgets(line,256,helpfile),"USAGE",5) != 0);
	fgets(line,256,helpfile);
	printf ("\n");
	while ((line[0] == ' ') || (line[0] == '\t') || (line[0] == '\n'))
		{
		printf ("%s",line);
		fgets(line,256,helpfile);
		}
	printf ("\n");
	fclose(helpfile);
	}
/*****************************************************************/

else

/*****************************************************************/
/*** Unless it cannot find the help file                       ***/
/*****************************************************************/

	{
	printf ("\n*** Error! Could not find %s help file in %s\n\n",program,path);
	}
/*****************************************************************/

free(helpfilename);
}
