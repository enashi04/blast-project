/*** countmatches  ***********************************************/
/*                                                               */
/* Counts numbers of matching words in sequence                  */
/*                                                               */
/*****************************************************************/
/*                                                               */
/*                                                               */
/*****************************************************************/

#include <stdio.h> 
#include "types.h"

int countmatches (SimPrf *first)
{
SimPrf *sim;
int n=0;

sim  = first;

while (sim != NULL)
	{
	n += sim->nmatch;
	sim = sim->next;
	}
return n;
}	
/*****************************************************************/
