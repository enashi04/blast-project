/*** maskprofil ************************************************/
/*                                                               */
/*   Applies user-defined mask to profile                        */
/*   returns a pointer to the masked profile                     */
/*                                                               */
/*****************************************************************/
/* double *profil       : profile                                */
/* int     length       : length of profile                      */
/* char   *mask         : user-defined mask                      */
/*                                                               */
/* double *masked       : masked profile                         */
/* double *ptr          : pointer to current position            */
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"


double *maskprofil(double *profil, int length, char *mask)
{
int i;
double *ptr, *masked;

masked  = (double *) (malloc (length * sizeof (double)));

if (strlen(mask) < length)
        {
        printf ("**** Warning **** mask shorter than profile !!! \n");
        }

if (strlen(mask) > length)
        {
        printf ("**** Warning **** mask longer than profile !!! \n");
        }


for (i = 0; i < MIN(length,strlen(mask));i++)
        {
        ptr = (double *)(masked + i);

#ifdef WWW
        if (*(mask + i ) == '0')
#else
        if ((*(mask + i) > 'Z') || (*(mask +i) == '0'))
#endif
                {
                *ptr = 0;
                }
        else
                {
#ifdef DEBUG
                printf ("%d %c %f\n",i,*(mask + i),*(profil + i));
#endif
                *ptr = *(profil + i);
                }
#ifdef DEBUG
                printf ("%d %c %f\n",i,*(mask + i),*(profil + i));
#endif
        }

return masked;
}