/*** consseq *****************************************************/
/*                                                               */
/*   Creates a FASTA formatted sequence which residues are       */
/*   those having a positive value in the profile.               */
/*   No return value.                                            */
/*                                                               */
/*****************************************************************/
/* double *profil       : profile                                */
/* int     length       : length of profile                      */
/* char   *filename     : output file name                       */
/* char   *conserved    : table of residues found in any HSP     */
/*                                                               */
/* double *ptr          : pointer to profile successive positions*/
/* char   *seqfilename  : sequence file name                     */
/* FILE   *seqfile      : sequence file handler                  */
/*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LARGEUR 50

void consseq(double *profil, int length, char *filename, char *conserved)
{
	int i;//, j, k;
	double *ptr;
	char *seqfilename;
	FILE *seqfile;

	/******************************************************************/
	/***** Derives sequence file name from output file name ***********/
	/******************************************************************/

	seqfilename = (char *)malloc(strlen(filename) + 5);
	sprintf(seqfilename, "%s.csq", filename);

	/******************************************************************/

	/******************************************************************/
	/***** Writes sequence in FASTA format into sequence file *********/
	/******************************************************************/

	seqfile = fopen(seqfilename, "w");

	fprintf(seqfile, ">%s\n", filename);
	for (i = 0; i < length; i++)
	{
		ptr = (double *)(profil + i);

		/**********************************************************/
		/*** if profile is positive at current position then ******/
		/*** write corresponding residue or else write x     ******/
		/**********************************************************/

		if (*ptr > 0)
		{
			fprintf(seqfile, "%c", *(conserved + i));
		}
		else
		{
			fprintf(seqfile, "x");
		}
		/**********************************************************/

		if (((i + 1) % LARGEUR) == 0)
			fprintf(seqfile, "\n");
	}
	/******************************************************************/

	fclose(seqfile);
	free(seqfilename);
}
/*****************************************************************/
