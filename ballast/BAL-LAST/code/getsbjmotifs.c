/*** getsbjmotifs ************************************************/
/*                                                               */
/*   Gets potential motifs in subject sequence                   */
/*   i.e. those HSP parts that overlap defined motifs and score  */
/*   positively                                                  */
/*   returns the list of subject motifs                          */
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "types.h"

double scalaire (double *profil1, double *profil2, int begin, int end, int hspbegin, int hspend, int offset);
int     overlap     (Motif *motif, SimPrf *sim);


Sbjmot *getsbjmotifs (Motif *motif, SimPrf *sim, double *profil, char type)
{
Sbjmot *firstsbjmot=NULL;
Sbjmot *sbjmot;
SimPrf *simptr;
Motif  *motifptr;
int f;


simptr = sim;
 
if (type == 'n') {f=3;} else {f=1;}
 
while (simptr != NULL)
	{
	simptr->maxscore = 0;

#ifdef DEBUG
        printf ("HSP %d|%d (%d|%d)\n",simptr->begin,simptr->end,simptr->begdb,simptr->enddb);
#endif
	motifptr = motif;
	while (motifptr != NULL)
		{
 
#ifdef DEBUG
        printf ("     %d|%d (%8.2f)\n",motifptr->begin,motifptr->end,motifptr->maxscore);
#endif
 
		if (overlap (motifptr, simptr) == 1)
			{
			simptr->maxscore += motifptr->maxscore;

			if (firstsbjmot == NULL)
				{
				sbjmot = (Sbjmot *) malloc (sizeof (Sbjmot));
				firstsbjmot = sbjmot;
				}
			else
				{
				sbjmot->next = (Sbjmot *) malloc (sizeof (Sbjmot));
				sbjmot = sbjmot->next;
				}

			sbjmot->hsp   = simptr;
			sbjmot->motif = motifptr;
			
			sbjmot->next = NULL;
			sbjmot->alnscore = 0;
			sbjmot->alnprev  = NULL;
			sbjmot->alnnext  = NULL;

                        sbjmot->begdb = MAX (simptr->begdb, simptr->begdb + f * (motifptr->begin - simptr->begin));
                        sbjmot->enddb = MIN (simptr->enddb, simptr->begdb + f * (motifptr->end - simptr->begin));
			sbjmot->begin = MAX (motifptr->begin, simptr->begin);
			sbjmot->end   = MIN (motifptr->end  , simptr->end  );

			sbjmot->score = scalaire (simptr->prf,profil,motifptr->begin,motifptr->end,sbjmot->begin,sbjmot->end,simptr->begin);

#ifdef DEBUG
        printf ("      --->%d|%d (%d|%d) score : %f\n",sbjmot->begin,sbjmot->end,sbjmot->begdb,sbjmot->enddb,sbjmot->score);
	if ((sbjmot->end - sbjmot->begin) != (motifptr->end - motifptr->begin)) printf ("      !!!!Tronqué\n");
#endif
			}

		motifptr = motifptr->next;
		}
	simptr = simptr->next;
	}

return firstsbjmot;
}

