#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "readargs.h"
#include "types.h"

/*#define DEMIFEN 4*/
#define DEMIFEN demifen

//extern void profilplot(double *profil, double *profil2, int length, char *filename, char *conserved, SeqHSP *first);

double *smoothprofil(double *profil, int length, char *cons)
{
	int demifen;
	double *smoothed, *ptr;
	double m;
	int i, j, n;
	printf("la valeur de profil+j est :%lf\n", *profil);

	//printf("le profil est %lf \n", *profil);
	if (getargint("-smooth", &demifen) == NULL)
		demifen = 6;//augmentation de la fenetre à 6

	/*****************************************************************/
	/*** Smooth 'profile'                                         ****/
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
				//printf("Alors la valeur de cons est %c\n",*(cons + j) );
				if (*(cons + j) != '.')
				{

					m += *(profil + j);
					n++;
					
				}
			}
		}
		ptr = (double *)(smoothed + i); //c'est ici que la valeur du lissage est initiée
		//printf("lisse : %lf\n", *smoothed);

		if ((n != 0) && (*(cons + i) != '.'))
		{
			m = m / (double)n;
			//printf("La valeur de m et n sont  : %lf - %u\n", m, n);
		}
		else
		{
			m = 0;
		}
		*ptr = m;
		//printf("ptr : %lf\n", *ptr);
	}
	printf("lissage : %lf\n", *smoothed);
	return smoothed;
}
