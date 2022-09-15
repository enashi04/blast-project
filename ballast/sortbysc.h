/*** sortbysc ****************************************************/
/*                                                               */
/*   Sorts sequences producing HSPs by BALLAST score             */
/*   Returns Sequence producing HSP with highest score           */
/*                                                               */
/*****************************************************************/
/* SeqHSP *seqres       : sequence producing HSPs                */
/*                                                               */
/* SeqHSP *first        : first sequence producing HSPs          */
/* SeqHSP *last         : last sequence producing HSPs           */
/* SeqHSP *ordered      : ordered sequence producing HSPs        */
/*****************************************************************/

#include <stdio.h>
#include "types.h"


SeqHSP *sortbysc(SeqHSP *seqres)
{
SeqHSP *ordered;
SeqHSP *first, *last;

int ok;

first = seqres;

first->sortednext = NULL;
first->sortedprev = NULL;
last = first;
seqres = seqres->next;

#ifdef DEBUG
        printf ("first = %s\n",first->name);
#endif

while (seqres != NULL)
        {
        ok = 0;
        if (seqres->sc > first->sc)
                {
                seqres->sortednext = first;
                seqres->sortedprev = NULL;
                first->sortedprev  = seqres;
                first = seqres;
#ifdef DEBUG
        printf ("new first = %s (suivant = %s / check %s)\n",first->name,first->sortednext->name,first->sortednext->sortedprev->name);
#endif
                }
        else
                {
                ordered = first->sortednext;

                while (ordered != NULL)
                        {
                        if (seqres->sc > ordered->sc)
                                {
                                seqres->sortednext = ordered;
                                seqres->sortedprev = ordered->sortedprev;
                                (ordered->sortedprev)->sortednext = seqres;
                                ordered->sortedprev = seqres;
                                ok = 1;
#ifdef DEBUG
        printf ("%s inserted between %s and %s\n",seqres->name,seqres->sortedprev->name,seqres->sortednext->name);
        printf ("check %s and %s\n",seqres->sortedprev->sortednext->name,seqres->sortednext->sortedprev->name);
#endif
                                break;
                                }
                        ordered = ordered->sortednext;
                        }

                if (ok == 0)
                        {
                        seqres->sortedprev = last;
                        seqres->sortednext = NULL;
                        last->sortednext   = seqres;
                        last = seqres;
#ifdef DEBUG
        printf ("new last  = %s (previous = %s / check %s)\n",last->name,last->sortedprev->name,last->sortedprev->sortednext->name);
#endif
                        }
                }

        seqres = seqres->next;
        }
#ifdef DEBUG
        printf ("Sorted, first = %s, last = %s\n",first->name, last->name);
#endif

return first;
}