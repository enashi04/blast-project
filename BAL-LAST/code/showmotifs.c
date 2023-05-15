/*** showmotifs **************************************************/
/*                                                               */
/*   Shows all motifs as patterns or profile                     */
/*   No return value.                                            */
/*                                                               */
/*****************************************************************/
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

void showmotifs(Motif *motif, char type, float thr1, float thr2)
{
	Motif *motifptr;
	Pssm *profile;
	int pos, i;
	char pattern[256];//, res[128];

	motifptr = motif;

	while (motifptr != NULL)
	{
		profile = motifptr->profile;
		pos = motifptr->begin;
		printf("%5d:  ", pos + 1);
		while (profile != NULL)
		{
			strcpy(pattern, "");
			for (i = 0; i < 26; i++)
			{
				if (profile->w[i] / profile->n > thr1)
				{
					sprintf(pattern, "%s%c", pattern, (i + 65));
				}
			}
			for (i = 0; i < 26; i++)
			{
				if ((profile->w[i] / profile->n <= thr1) && (profile->w[i] / profile->n > thr2))
				{
					sprintf(pattern, "%s%c", pattern, (i + 97));
				}
			}
			if (strlen(pattern) > 1)
			{
				printf("[%s]", pattern);
			}
			else
			{
				if (strlen(pattern) == 1)
				{
					printf("%s", pattern);
				}
				else
				{
					printf("x");
				}
			}
			profile = profile->next;
			if (profile != NULL)
				printf("-");
		}
		motifptr = motifptr->next;
		printf("\n");
	}
}
/*****************************************************************/
