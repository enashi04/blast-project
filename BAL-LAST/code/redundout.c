/*** redundout ***************************************************/
/*                                                               */
/*   Removes redundancy in sequences producing HSPs by looking   */
/*   for twin sequences. Twins are sequences that produce exactly*/
/*   the same HSPs.                                              */
/*   Returns the first sequence producing HSPs                   */
/*                                                               */
/*****************************************************************/
/* SeqHSP *seqres       : first sequence producing HSPs          */
/*                                                               */
/* SeqHSP *seqres1      : first sequence for comparison purpose  */
/* SeqHSP *seqres2      : 2nd sequence for comparison purpose    */
/* SeqHSP *seqrestmp    : tmp sequence for exchange purpose      */
/* float   threshold    : output threshold                       */
/* SimPrf *s1           : HSP from seqres1                       */
/* SimPrf *s2           : corresponding HSP from seqres2         */
/* int     different    : flag to say s1 and s2 are different    */
/* char    shortdesc[50]: short description of twin sequence     */
/*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readargs.h"
#include "types.h"

SeqHSP *redundout(SeqHSP *seqres)
{
	SeqHSP *seqres1, *seqres2;//, *seqrestmp;
	SimPrf *s1, *s2;
	char shortdesc[50];
	int different;
	float threshold;

	if (getargfloat("-t", &threshold) == NULL)
		threshold = 0.0;

	seqres1 = seqres;

	/*****************************************************************/
	/*** For each sequence scoring above threshold                 ***/
	/*****************************************************************/

	while ((seqres1 != NULL) && (seqres1->sortednext != NULL) && (seqres1->sc > threshold))
	{
		seqres2 = seqres1->sortednext;

		/**********************************************************/
		/*** For each sequence having the same score as seqres1 ***/
		/**********************************************************/

		while ((seqres2 != NULL) && (seqres1->sc == seqres2->sc))
		{
			different = 0;

			/**************************************************/
			/*** check if matching HSPs are different       ***/
			/**************************************************/

			for (s1 = seqres1->sim, s2 = seqres2->sim; (s1 != NULL) && (s2 != NULL); s1 = s1->next, s2 = s2->next)
			{
				if ((s1->score > 0) && (s2->score > 0) && (strcmp(s1->hsp, s2->hsp) != 0))
				{
					different = 1;
					break;
				}
			}

			if (((s1 == NULL) && (s2 != NULL)) || ((s1 != NULL) && (s2 == NULL)))
				different = 1;
			/**************************************************/

			/**************************************************/
			/*** If not then then remove the twin sequences ***/
			/*** and add their name to the list of twins    ***/
			/**************************************************/

			if (different == 0)
			{
				strncpy(shortdesc, seqres2->desc, 46);
				shortdesc[46] = '\n';
				shortdesc[47] = '\0';
				(seqres2->sortedprev)->sortednext = seqres2->sortednext;
				if (seqres2->sortednext != NULL)
					(seqres2->sortednext)->sortedprev = seqres2->sortedprev;
				if (seqres1->twins != NULL)
				{
					seqres1->twins = (char *)realloc(seqres1->twins, strlen(seqres1->twins) + 63);
					strcat(seqres1->twins, "               ");
				}
				else
				{
					seqres1->twins = (char *)malloc(63);
					strcpy(seqres1->twins, "               ");
				}
				strcat(seqres1->twins, shortdesc);
			}
			/**************************************************/

			seqres2 = seqres2->sortednext;
		}
		/**********************************************************/
		seqres1 = seqres1->sortednext;
	}
	/*****************************************************************/

	return seqres1;
}
