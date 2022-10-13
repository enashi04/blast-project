/*** freesbjmotifs ***********************************************/
/*                                                               */
/* Frees Subject motif structures                                */
/*                                                               */
/*****************************************************************/
/*                                                               */
/*                                                               */
/*****************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

void freesbjmotifs (Sbjmot *first)
{
Sbjmot *sbjmot,*prev;

sbjmot = first->sortednext;
prev = first;

while (sbjmot != NULL)
	{
	free (prev);
	prev = sbjmot;
	sbjmot = sbjmot->sortednext;
	}
}	
/*****************************************************************/
