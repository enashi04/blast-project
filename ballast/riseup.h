/*** riseup  *****************************************************/
/*                                                               */
/*   Rises up sequences producing HSPs which have an E(N)        */
/*   value of less than 1e-03 in order to keep them higher       */
/*   in the list no matter what their score is                   */
/*                                                               */
/*****************************************************************/
/*                                                               */
/* SeqHSP **first        : first sequence producing HSPs          */
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include "macros.h"
#include "types.h"

void  riseup (SeqHSP **first)
{
SeqHSP *seqres, *next, *insert;

insert = *first;

while ((insert != NULL) && (insert->prob <= ELIMIT))
        {
        insert = insert->sortednext;
        }

#ifdef DEBUG
        if (insert != NULL)
                {
                printf ("Point d'insertion : %s (%f)\n",insert->name,insert->prob);
                if (insert->sortedprev != NULL) { printf ("           �tait entre %s et ",insert->sortedprev->name);}
                else { printf ("           �tait au d�but, juste avant ");}
                if (insert->sortednext != NULL) { printf ("%s\n",insert->sortednext->name); }
                else { printf ("la fin\n"); }
                }
#endif

seqres = insert;

while (seqres != NULL)
        {

#ifdef DEBUG
        printf ("\n%s (%f)\n",seqres->name,seqres->prob);
#endif
        next = seqres->sortednext;
        if (seqres->prob <= ELIMIT)
                {


#ifdef DEBUG
        printf ("\nRemonter (%f/%f)\n",seqres->prob,seqres->sc);
        if (seqres->sortedprev != NULL) { printf ("           �tait entre %s et ",seqres->sortedprev->name);}
        else { printf ("           �tait au d�but, juste avant ");}
        if (seqres->sortednext != NULL) { printf ("%s\n",seqres->sortednext->name); }
        else { printf ("la fin\n"); }
#endif


                seqres->risedup = '^';
                seqres->sortedprev->sortednext = seqres->sortednext;
                if (seqres->sortednext != NULL) seqres->sortednext->sortedprev = seqres->sortedprev;

                seqres->sortednext = insert;
                seqres->sortedprev = insert->sortedprev;
                if (insert->sortedprev != NULL) { insert->sortedprev->sortednext = seqres;}
                else {*first = seqres;}
                insert->sortedprev = seqres;



#ifdef DEBUG
        if (seqres->sortedprev!= NULL) { printf ("           ====> entre %s et ",seqres->sortedprev->name);}
        else { printf ("           ====> au d�but, juste avant ");}
        if (seqres->sortednext != NULL) { printf ("%s\n",seqres->sortednext->name); }
        else { printf ("la fin\n"); }
        printf ("Insertion : %s (%f)\n",insert->name,insert->prob);
        if (insert->sortedprev != NULL) { printf ("           entre %s et ",insert->sortedprev->name);}
        else { printf ("           au d�but, juste avant ");}
        if (insert->sortednext != NULL) { printf ("%s\n",insert->sortedprev->name,insert->sortednext->name); }
        else { printf ("la fin\n"); }
#endif


                }
        seqres = next;
        }
}