/*** readprofil **************************************************/
/*                                                               */
/*   Reads an existing profile from a file                       */
/*   no return value                                             */
/*                                                               */
/*****************************************************************/
/* char   *filename     : profile file name                      */
/* char   *profile      : pointer to the profile                 */
/*                                                               */
/* char   line[80]      : last line read                         */
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void    readprofil (char *filename, double *profil)
{
char line [80];
FILE *file;
double *prf;
int i=0, j;

file = fopen (filename, "r");

fgets (line,80,file);
fgets (line,80,file);

#ifndef WWW
while (strncmp (line,"maximum",7) != 0)
	{
	fgets (line,80,file);
	}
j = 11;
#else
j = 1;
#endif

while (fgets (line,80,file)!= NULL)
	{
	prf = (double *) (profil + i);
	sscanf ((char * )(&line[j]),"%lf",prf);
	i++;
#ifdef DEBUG
	printf ("%3d : %f \n",i,*prf);
#endif
	}
}

/*****************************************************************/
