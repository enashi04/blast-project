/*** consmotif ***************************************************/
/*                                                               */
/*   Determines consensus pattern from mmotif                    */
/*   No return value.                                            */
/*                                                               */
/*****************************************************************/
/*                                                               */
/*****************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"


void consmotif (Motif *motif, char *seq, int offset, float weight, float maxweight)
{
Pssm   *profile;
int     pos, i;


if (motif->profile == NULL)
	{
	motif->profile = (Pssm *) malloc (sizeof(Pssm));
	profile = motif->profile;
	for (i=0; i<26;i++) profile->w[i] = 0.0;
	profile->n = 0;
	for (pos = motif->begin; pos < motif->end; pos++)
		{
		profile->next = (Pssm *) malloc (sizeof(Pssm));
		profile->next->prev = profile;
		profile = profile->next;
		for (i=0; i<26;i++) profile->w[i] = 0.0;
		profile->n = 0;
		}
	profile->next=NULL;
	}
profile = motif->profile;

for (pos = motif->begin; pos < motif->begin+offset; pos++)
	{
	profile->n += maxweight;
	profile = profile->next;
	}

for (i = pos; i < pos + strlen(seq); i++)
	{
	profile->w[(int)(*(seq+i-pos))-65] += weight;
	profile->n += maxweight;
	profile = profile->next;
	}

for (i = pos + strlen(seq); i < motif->end; i++)
	{
	profile->n += maxweight;
	profile = profile->next;
	}
}	
/*****************************************************************/
