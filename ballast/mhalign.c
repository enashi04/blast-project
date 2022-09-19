/*** MHalign  ****************************************************/
/*                                                               */
/*   Aligns HSPs and motifs                                      */
/*   returns the score of best alignment                         */
/*                                                               */
/*****************************************************************/
/*                                                               */
/* Motif  *motif        : list of proposed motifs                */
/* SimPrf *sbjmot          : list of HSPs from current sequence     */
/* double *profil       : profile                                */
/* char    type         : type of sequence ('N' or 'P')          */
/*                                                               */
/* double  score        :                                        */
/* double  s            :                                        */
/* double  sensepenalty :                                        */
/* double  distpenalty  :                                        */
/* double  maxscore     :                                        */
/* double  prevscore    :                                        */
/* SimPrf *sbjmotptr       : pointer to current HSP structure       */
/* Motif  *motifptr     : pointer to current motif               */
/* Motif  *prevmotif    : pointer to previous motif              */
/* int     begin        : start position of overlap between HSP  */
/*                        and motifs                             */
/* int     end          : end position of overlap between HSP    */
/*                        and motifs                             */
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include "macros.h"
#include "types.h"


double MHalign (Motif *motif, Sbjmot *sbjmot, double *profil, char type, Sbjmot **first)
{
double sensepenalty;
double distpenalty;

double highscore;

Sbjmot *sbjmotptr,*previous;
Sbjmot *best;

best = sbjmot;
best->alnscore = best->score;
sbjmotptr = sbjmot->sortednext;

#ifdef DEBUG
printf ("\nQuery : %d|%d <=> Sbjct : %d|%d ",sbjmot->motif->begin,sbjmot->motif->end,sbjmot->begdb,sbjmot->enddb);
printf (" as first motif %5.3f/%5.3f\n",sbjmot->score,sbjmot->motif->maxscore);
#endif

while (sbjmotptr != NULL)
	{
#ifdef DEBUG
printf ("\nQuery : %d|%d <=> Sbjct : %d|%d ",sbjmotptr->motif->begin,sbjmotptr->motif->end,sbjmotptr->begdb,sbjmotptr->enddb);
#endif
#ifdef DEBUG
		printf ("\n  as first motif %5.3f/%5.3f",sbjmotptr->score,sbjmotptr->motif->maxscore);
#endif
	sbjmotptr->alnscore = sbjmotptr->score;
	sbjmotptr->alnprev=NULL;

	if (sbjmotptr->score > 0.0 )
		{
		highscore = sbjmotptr->score;

		previous = sbjmotptr->sortedprev;
	
		while (previous != NULL)
			{
			if (previous->motif->begin < sbjmotptr->motif->begin)
				{
#ifdef DEBUG
				printf ("\n  via motif %d|%d :  %5.3f/%5.3f (%5.3f) (%5.3f : ",previous->begdb,previous->enddb,previous->score,previous->motif->maxscore, previous->alnscore,best->alnscore);
#endif
				sensepenalty = 0.0;
				if (sbjmotptr->hsp->sens != previous->hsp->sens) sensepenalty = SPENALTY;
#ifdef DEBUG
				printf (" %5.3f ) ",sbjmotptr->score + previous->alnscore - sensepenalty);
#endif
				if ((sbjmotptr->score + previous->alnscore - sensepenalty) > highscore)
					{
					sbjmotptr->alnscore = sbjmotptr->score + previous->alnscore - sensepenalty;
					highscore = sbjmotptr->alnscore;
					sbjmotptr->alnprev=previous;
#ifdef DEBUG
					printf (" OK ");
#endif
					}
				}
			previous = previous->sortedprev;
			}
		}
	sbjmotptr = sbjmotptr->sortednext;
	}
#ifdef DEBUG
		printf ("\n\n");
#endif

sbjmotptr = sbjmot->sortednext;

while (sbjmotptr != NULL)
        {
	if (sbjmotptr->alnscore > best->alnscore) best = sbjmotptr;
	sbjmotptr = sbjmotptr->sortednext;
	}

sbjmotptr = best;

while (sbjmotptr != NULL)
	{
#ifdef DEBUG
	printf ("HSP %d|%d (%d|%d) : %8.2f\n",sbjmotptr->hsp->begin,sbjmotptr->hsp->end,sbjmotptr->hsp->begdb,sbjmotptr->hsp->enddb,sbjmotptr->hsp->score);
#endif
	sbjmotptr->hsp->score += sbjmotptr->score;
	sbjmotptr->hsp->nmatch++;
	*first = sbjmotptr;
	if (sbjmotptr->alnprev != NULL) sbjmotptr->alnprev->alnnext = sbjmotptr;
	sbjmotptr = sbjmotptr->alnprev;
	}
return best->alnscore;
}	
