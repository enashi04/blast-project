/**
 * @file loadhspv2.c
 * @author Ihsane E.
 * @brief AVEC LE MOINS DE POINTEURS POSSIBLES Y'EN A MARRE DES ALLOCATIONS MEMOIRES
 * @version 0.1
 * @date 2022-09-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "macros.h"
#include "readargs.h"
#include "types.h"

extern int filter(char *seqhsp, char *seq);
extern SimPrf *handlegaps(SimPrf *simprf);

#define NETRA 0

double *loadHSP(SeqHSP *seqres, FILE *file, char *line, int length, char *conserved, double *maxprofile, char type)
{
	double maxp;
	double *ptr;
	SimPrf *simprf;
	if(getargdouble("-maxp", &maxp)==NULL){
		maxp=DEFMAXP;
	}

	seqres->sc=0;
	seqres->nmatch=0;
	seqres->type=type;
	seqres->risedup=' ';
	seqres->sim = (SimPrf *)malloc(sizeof(SimPrf));
	simprf= seqres->sim;
	simprf->score=0;
	simprf->maxscore=0;
	simprf->nmatch=0;
	seqres->p = 1;
	seqres->prob =maxp;

	double *profil = (double *)malloc(sizeof(double)*length);

	for(int i =0; i<length; i++){
		ptr=profil+i;
		*ptr=0;
	}

	char *outtext = (char *)malloc(strlen(line)+1);
	strcpy(outtext,line);

	if (line[0] == '>')
	{
		strcpy(line, &line[1]);
	}

	if (strlen(line) <= 80)
	{
		for (int i = strlen(line) - 1; i < 80; i++)
		{
			line[i] = ' ';
		}
	}
	line[80] = '\0';

	seqres->desc = malloc(strlen(line)+1);

}
	