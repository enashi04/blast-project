/*** extractmotifs ***********************************************/
/*                                                               */
/*   Extracts motifs from profile                                */
/*   No return value.                                            */
/*                                                               */
/*****************************************************************/
/* double *profil       : profile                                */
/* int     length       : length of profile                      */
/* char   *conserved    : table of residues found in any HSP     */
/*                                                               */
/* Motif  *firstmotif   : first extracted motif                  */
/* Motif  *motif        : pointer to current motif               */
/*                                                               */
/* int     begin        : current motif start position           */
/* int     end          : current motif end position             */
/*                                                               */
/* double  score        : score of motif                         */
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "macros.h"
#include "types.h"

#define MINSCORE 0.01

Motif *extractmotifs(double *profil, int length, char *conserved)
{
    Motif *firstmotif, *motif;
    int i, j, n, begin, end;
    int number = 1;
    int maxpos;
    double maxprofvalue;
    double score;

    firstmotif = NULL;
    motif = NULL;

#ifndef WWW
    printf("\nSuggested Words : \n\n");
#endif

    i = 0;

    while (i < length)
    {
        while ((i < length) && (*(profil + i) == 0))
        {
            i++;
        }
        n = 0;
        score = 0;
        maxprofvalue = 0;
        maxpos = i;
        begin = i;
        while ((i < length) && (*(profil + i) != 0))
        {
            score += *(profil + i) * ID;
            if (*(profil + i) > maxprofvalue)
            {
                maxpos = i + 1;
                maxprofvalue = *(profil + i);
            }
#ifdef DEBUG
            printf(" %d %8.2f (%8.2f)\n", i, *(profil + i), score);
#endif
            n++;
            i++;
        }
        end = i;
        if ((n > MINWORD) && (score > MINSCORE))
        {
            if (motif == NULL)
            {
                motif = (Motif *)malloc(sizeof(Motif));
                firstmotif = motif;
            }
            else
            {
                motif->next = (Motif *)malloc(sizeof(Motif));
                (motif->next)->prev = motif;
                motif = motif->next;
            }

            motif->begin = begin;
            motif->end = end - 1;
            motif->n = number++;
            motif->hsp = NULL;
            motif->score = 0;
            motif->maxscore = score;
            motif->next = NULL;
            motif->profile = NULL;
            motif->peak = maxpos;

#ifndef WWW
            printf("\t%4d - %4d\t: ", begin + 1, end);
            for (j = begin; j < end; j++)
            {
                printf("%c", *(conserved + j));
            }
            printf("  \t:%8.2f\n", score);
            printf("\n");
#endif
        }
    }
    return firstmotif;
}
/*****************************************************************/
