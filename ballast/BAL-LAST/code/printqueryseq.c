/*** printqueryseq ***********************************************/
/*                                                               */
/*   Prints query sequence into Ballast result file.             */
/*   Residues which are never found in a HSP are replaced by     */
/*   x's and only residues for which the 'profile' is positive   */
/*   are written in capitals.                                    */
/*   '*', ':' and '.' mark those position of positive 'profile'  */
/*   according to their relative value.                          */
/*                                                               */
/*****************************************************************/
/* FILE   *outfile      : output file                            */
/* char   *conserved    : residues found in at least one HSP     */
/* double *profil       : 'profile'                              */
/* int     length       : length of the query sequence           */
/*                                                               */
/* double  max          : max value of 'profile'                 */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "readargs.h"
#include "types.h"

void    printqueryseq (FILE *outfile, char *conserved, double *profil, int length)
{
int i,j,min;
double max=0;
char *ptr;

int pval;
float p;


/*****************************************************************/
/**** Determines the max value of the profile              *******/
/*****************************************************************/

for (i=0; i<length; i++)
	{
	if (max < *(profil + i)) max = *(profil + i);
	}
/*****************************************************************/


/*****************************************************************/
/**** Writes sequence in 50-residue long lines             *******/
/*****************************************************************/

for (i=0; i<length ; i+=50)
	{
	min = ((length)<(i+50)?(length):(i+50));
	fprintf (outfile,"      ");

	/********************************************************/
	/**** Writes line indicating the 'profile' value  *******/
	/********************************************************/

	for (j=i;j<min;j++)
		{
		p = ((*(profil + j) * 10) / max);
		pval = p;
		if (pval == 10.0) pval = 9;
		if (p > 0) 
			{
			fprintf (outfile,"%d",pval);
			}
		else
			{
			fprintf (outfile," "); 
			ptr = (char *)(conserved + j);
			*ptr = tolower(*ptr);
			}
		}
	/********************************************************/


	/********************************************************/
	/**** Writes sequence line                        *******/
	/********************************************************/

	fprintf (outfile,"\n%4d  ",i+1);
	for (j=i;j<min;j++)
		{
		ptr = (char *)(conserved + j);
		if (*ptr == '.') *ptr = 'x';
		fprintf (outfile,"%c",*ptr);
		}
	fprintf (outfile,"%4d\n\n",j);
	}
	/********************************************************/

/*****************************************************************/
	
fprintf (outfile,"==============================================================================\n\n");
return ;
}

