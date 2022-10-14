/*** sortbybegdb *************************************************/
/*                                                               */
/*   Sorts HSPs by position of the first overlapping motif       */
/*   Returns first HSP                                           */
/*                                                               */
/*****************************************************************/
/*                                                               */
/* Sbjmot *motifdb          : HSPs to be sorted                      */
/* Sbjmot *ordered      : reordered HSPs                         */
/* Sbjmot *first        : pointer to first ordered HSP           */
/* Sbjmot *last         : pointer to last  ordered HSP           */
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include "types.h"


Sbjmot *sortbybegdb (Sbjmot *motifdb)
{
Sbjmot *ordered;
Sbjmot *first, *last;

int ok;

first = motifdb;

first->sortednext = NULL;
first->sortedprev = NULL;
last = first;
motifdb = motifdb->next;

while (motifdb != NULL)
	{
	ok = 0;
	if (motifdb->begdb < first->begdb)
		{
		motifdb->sortednext = first;
		motifdb->sortedprev = NULL;
		first->sortedprev  = motifdb;
		first = motifdb;
		}
	else
		{
		ordered = first->sortednext;

		while (ordered != NULL)
			{
			if (motifdb->begdb < ordered->begdb)
				{
				motifdb->sortednext = ordered;
				motifdb->sortedprev = ordered->sortedprev;
				(ordered->sortedprev)->sortednext = motifdb;
				ordered->sortedprev = motifdb;
				ok = 1;
				break;
				}
			ordered = ordered->sortednext;
			}

		if (ok == 0)
			{
			motifdb->sortedprev = last;
			motifdb->sortednext = NULL;
			last->sortednext   = motifdb;
			last = motifdb;
			}
		}

	motifdb = motifdb->next;
	}
return first;
}	
