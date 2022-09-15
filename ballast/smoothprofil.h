/*** smoothprofil ************************************************/
/*                                                               */
/*   Smooths 'profile' to get an non linear base line            */
/*   The smoothed profile is then subtracted from the original   */
/*   one                                                         */
/*   Returns smoothed profile                                    */
/*                                                               */
/*****************************************************************/
/* double *profil       : profile                                */
/* int     length       : length of profile                      */
/* char   *cons         :                                        */
/*                                                               */
/* double *smoothed     : pointer to smoothed profile            */
/* double *ptr          : pointer to profile successive positions*/
/* double  m            : mean value of profile over window      */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "readargs.h"
#include "types.h"

/*#define DEMIFEN 4*/
#define DEMIFEN demifen

extern void    profilplot  (double *profil, double *profil2, int length, char *filename, char *conserved, SeqHSP *first);



double *smoothprofil(double *profil, int length, char *cons)
{
int demifen;
double *smoothed,*ptr;
double  m;
int i,j,n;

if (getargint ("-smooth",&demifen) == NULL ) demifen = 4;


/*****************************************************************/
/*** Smooth 'profile'                                         ****/
/*****************************************************************/

smoothed = (double *) malloc (length * sizeof (double));

for (i=0;i<length;i++)
        {
        m = 0;
        n=0;
        for (j=i-DEMIFEN;j<i+DEMIFEN+1;j++)
                {
                if((j>=0) && (j<length))
                        {
                        if(*(cons+j) != '.')
                                {
                                m+=*(profil+j);
                                n++;
                                }
                        }
                }
        ptr = (double *) (smoothed + i);
        if ((n!= 0) && (*(cons+i) != '.'))
                {
                m = m /(double) n;
                }
        else
                {
                m = 0;
                }
        *ptr = m;
        }
/*****************************************************************/

return smoothed;
}