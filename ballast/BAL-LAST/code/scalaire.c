/*** scalaire ****************************************************/
/*                                                               */
/*   Computes the scalar product of two profiles                 */
/*   Returns the resulting value                                 */
/*                                                               */
/*****************************************************************/
/* double *profil1      : profile r                              */
/* double *profil2      : profile 2                              */
/* int     begin        : begin  of profile 1                    */
/* int     end          : end    of profile 1                    */
/* int     hspbegin     : begin  of profile 2                    */
/* int     hspend       : end    of profile 2                    */
/*                                                               */
/* double  sc           : resulting value                        */
/*****************************************************************/


#include <stdio.h>
#include <math.h>
#include "macros.h"
#include "types.h"

double scalaire (double *profil1, double *profil2, int begin, int end, int hspbegin, int hspend, int offset)
{
int i;
double sc=0.0;

#ifdef DEBUG
printf ("\n");
printf ("Query : %d/%d   Subjct : %d/%d\n",begin,end,hspbegin,hspend);
#endif

for (i=begin;i< end+1;i++)
	{
	if ((hspbegin <= i) && (hspend >= i))
		{
#ifdef DEBUG
printf ("%d : %f %f#\n", i, (*(profil1 + i - hspbegin)),(*(profil2 + i )));
#endif
		sc = sc + ((double) (*(profil1 + i - offset))) * ((double) (*(profil2 + i )) );
		}
	else
		{
#ifdef DEBUG
printf ("%d : %f %f-\n", i, OUTOFHSP,(*(profil2 + i )));
#endif
		sc = sc + OUTOFHSP * ((double) (*(profil2 + i )) );
		}
	}

#ifdef DEBUG
printf ("==> %f\n", sc);
#endif
return sc;
}	
