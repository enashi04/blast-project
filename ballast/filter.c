/*** filter ******************************************************/
/*                                                               */
/*   Filters out HSP with very low complexity                    */
/*   Returns 1 if HSP is kept                                    */
/*   Returns 0 if HSP is filtered out                            */
/*                                                               */
/*****************************************************************/
/* char   *seqhsp       : HSP sequence to filter                 */
/* char   *seq          : Blast alignment middle line            */
/*                                                               */
/* int     n[26]        : number of time each Aa is found        */
/* int     deb[26]      : start position for each Aa reach window*/
/* int     dibenn[26]   : end position for each Aa reach window  */
/* float   max          : maximum Aa count in current window     */
/* int     aa           : Aa index (0 to 25)                     */
/* int     debut        : start position of low complexity subseq*/
/* int     fin          : end position of low complexity subseq  */
/* int     l            : total length of low complexity subseqs */
/*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******* Half of window length *********/
#define DEMIFEN 3

/******* Low complexity threshold ******/
#define SEUIL 29

/******* threshold for low complexity subseqs rejection *****/
#define LSEUIL (strlen(seqhsp) / 3)

/****************************************************************************/

int filter(char *seqhsp, char *seq)
{
	int n[26], deb[26], dibenn[26];
	int i, j, debut, fin;
	int l = 0;
	float max;
	int aa;
	char *ptr;

	/****************************************************************************/
	/***** For each position i in Seqhsp... *************************************/

	for (i = 0; i < strlen(seqhsp); i++)
	{
		for (j = 0; j < 26; j++)
		{
			n[j] = 0;
			deb[j] = 0;
		}
		max = 0;
		debut = -1;

		/*******************************************************************/
		/***** ... explore the Window of length 2*DEMIFEN centered on i ****/
		/***** to find the maximum number of times one single Aa is found **/
		/***** in Seqhsp. The smallest subseq where this number is maximum */
		/***** is considered as the lowest complexity subseq for this   ****/
		/***** window.                                                  ****/

		for (j = -DEMIFEN; j < DEMIFEN + 1; j++)
		{
			if (((i + j) > -1) && ((i + j) < strlen(seqhsp)))
			{
				ptr = (char *)(seqhsp + i + j);
				if (*ptr != '-')
				{
					if (*ptr != '*')
					{
						aa = (int)(*ptr) - 65;
					//	printf("aa = %u\n",aa);
					}
					else
					{
						aa = (int)('X') - 65;
						//printf("aa = %u\n",aa);

					}
				}

				if (n[aa] == 0)
					deb[aa] = i + j;
				dibenn[aa] = i + j;

				n[aa]++;

				if (n[aa] > max)
				{
					max = (float)n[aa];
					debut = deb[aa];
					fin = i + j;
				}
			}
		}
		/********************************************************************/

		max = 0;

		/********************************************************************/
		/***** Computes sort of complexity mesure for the whole Window ******/

		for (j = 0; j < 26; j++)
		{
			max += (float)(n[j] * n[j]);
		}
		/********************************************************************/

		/********************************************************************/
		/***** If this measure is higher than the threshold SEUIL then ******/
		/***** the lowest complexity subseq is suppressed by replacing ******/
		/***** all characters in BLAST alignment description by 'x'    ******/

		if ((max >= SEUIL) && (debut != -1))
		{
			for (j = debut; j <= fin; j++)
			{
				if ((j > 0) && (j < strlen(seqhsp)))
				{
					ptr = (char *)(seq + j);
					if (*ptr != 'x')
						l++;
					*ptr = 'x';
				}
			}
		}
		/********************************************************************/
	}
	//printf("l = %i\n",l);
	/****************************************************************************/

	/****************************************************************************/
	/**** If l, the total length of all lowest complexity subseqs is > LSEUIL ***/
	/**** then the whole HSP is considered as being of low complexity and non ***/
	/**** informative at all. It is therefore rejected.                       ***/

	if (l > LSEUIL)
	{
#ifdef DEBUG
		printf("\n************  Rejected  ************  \n");
		printf("************  %8d  ************  \n", l);
		printf("%s\n%s\n", seq, seqhsp);
#endif
		return (0);
	}
	else
	{
#ifdef DEBUG
		printf("\n************  Accepted  ************  \n");
		printf("************  %8d  ************  \n", l);
		printf("%s\n%s\n", seq, seqhsp);
#endif
		return (1);
	}
}
/****************************************************************************/
