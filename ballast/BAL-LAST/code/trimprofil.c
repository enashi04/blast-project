/*** trimprofil **************************************************/
/*                                                               */
/*   Trims profile in order to get only those regions of high    */
/*   interest.                                                   */
/*   Returns pointer to trimmed profile                          */
/*                                                               */
/*****************************************************************/
/* double *profiltotal  : original profile                       */
/* double *profil       : smoothed profile                       */
/* int     length       : length of profile                      */
/*                                                               */
/* double *ptr          : pointer to profile successive positions*/
/*                                                               */
/* double  dprof        : derivative of profile                  */
/* double  d2prof       : second derivative of profile           */
/* double *smoothed     : smoothed derived profile               */
/* double *trimmed      : trimmed profile                        */
/* float   seuil        : threshold for second derivative        */
/* double  m            : mean over window for smoothing         */
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "readargs.h"
#include "global.h"

#define DEFSEUIL 0.05
#define SEUIL seuil
#define DEMIFEN 4

double *trimprofil(double *profiltotal, double *profil, int length, char *cons)
{
	double *dprof, *d2prof;
	double *smoothed, *trimmed;
	double *ptr, pr1, pr2;
	int i, j, n;
	int i1, i2;
	double m;
	float seuil;

	if (getargfloat("-S", &seuil) == NULL)
		seuil = DEFSEUIL;
	seuil = -seuil;

	/*****************************************************************/
	/**** derives the smoothed profile *******************************/
	/*****************************************************************/

	dprof = (double *)malloc(length * sizeof(double));
	d2prof = (double *)malloc(length * sizeof(double));
	*dprof = 0;
	*d2prof = 0;
	ptr = (double *)(dprof + length - 1);
	*ptr = 0;
	ptr = (double *)(d2prof + length - 1);
	*ptr = 0;

	for (i = 1; i < length - 1; i++)
	{
		ptr = (double *)(dprof + i);
		if (*(cons + i) != '.')
		{
			i1 = -1;
			i2 = +1;
			if (*(cons + i + 1) == '.')
			{
				pr2 = *(profil + i);
				i2 = 0;
			}
			else
			{
				pr2 = *(profil + i + 1);
			}
			if (*(cons + i - 1) == '.')
			{
				pr1 = *(profil + i);
				i1 = 0;
			}
			else
			{
				pr1 = *(profil + i - 1);
			}
			*ptr = (pr2 - pr1) * 3.0 / (double)(i2 - i1 + 1);
			//formule
		}
		else
		{
			*ptr = 0.0;
		}
	}
	/*****************************************************************/

	/*****************************************************************/
	/**** smoothes the derived profile *******************************/
	/*****************************************************************/

	smoothed = (double *)malloc(length * sizeof(double));

	for (i = 0; i < length; i++)
	{
		m = 0;
		n = 0;
		for (j = i - DEMIFEN; j < i + DEMIFEN + 1; j++)
		{
			if ((j >= 0) && (j < length))
			{
				if (*(cons + j) != '.')
				{
					m += *(dprof + j);
					n++;
				}
			}
		}
		ptr = (double *)(smoothed + i);
		if ((n != 0) && (*(cons + i) != '.'))
		{
			m = m / (double)n;
		}
		else
		{
			m = 0;
		}
		*ptr = m;
	}

	/*****************************************************************/

	/*****************************************************************/
	/**** derives the derived profile ********************************/
	/*****************************************************************/

	for (i = 1; i < length - 1; i++)
	{
		ptr = (double *)(d2prof + i);
		if (*(cons + i) != '.')
		{
			i1 = -1;
			i2 = +1;
			if (*(cons + i + 1) == '.')
			{
				pr2 = *(smoothed + i);
				i2 = 0;
			}
			else
			{
				pr2 = *(smoothed + i + 1);
			}
			if (*(cons + i - 1) == '.')
			{
				pr1 = *(smoothed + i);
				i1 = 0;
			}
			else
			{
				pr1 = *(smoothed + i - 1);
			}
			*ptr = (pr2 - pr1) * 3.0 / (double)(i2 - i1 + 1);
		}
		else
		{
			*ptr = 0.0;
		}
	}

	/*****************************************************************/

	/*****************************************************************/
	/**** keeps only negative second derivative regions (= peaks) ****/
	/*****************************************************************/

	trimmed = (double *)malloc(length * sizeof(double));

	for (i = 0; i < length; i++)
	{
		ptr = (double *)(trimmed + i);
		if (*(d2prof + i) >= SEUIL)
		{
			*ptr = 0;
		}
		else
		{
			*ptr = *(profiltotal + i);
		}
		
		//printf ("%d %c : %f %f (%f) %f\n",i,*(cons +i),*(profil+i),*(smoothed+i),*(dprof+i),*(d2prof+i));
		
	}

	/*****************************************************************/

	free(dprof);
	free(d2prof);
	free(smoothed);
	return trimmed;
}
