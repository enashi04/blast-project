/*** overlap  ****************************************************/
/*                                                               */
/*   Do the motif and HSP overlap ?                              */
/*   returns 1 if they do                                        */
/*   returns -1 if they don't                                    */
/*                                                               */
/*****************************************************************/
/*                                                               */
/* Motif  *motif        : current motif                          */
/* SimPrf *sim          : current HSP                            */
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include "macros.h"
#include "types.h"

int overlap (Motif *motif, SimPrf *sim)
{
int reponse = -1;

if ((sim->end - motif->begin > MINWORD) && (motif->end - sim->begin > MINWORD)) reponse = 1;

return reponse;
}