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

    //printf("\nSuggested Words : \n\n");
    i = 0;
    //créer une chaine de caractère pour stocker les motifs
    char *stock_motifs = (char *)malloc(length * sizeof(char));

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
            char s_begin[5], s_end[5];
            sprintf(s_begin,"%4d",begin + 1);
            sprintf(s_end,"%4d",end);
            strcat(stock_motifs,s_begin);
            strcat(stock_motifs,"-");
            strcat(stock_motifs,s_end);
            strcat(stock_motifs,": ");
            //printf("\t%4d - %4d\t: ", begin + 1, end);
            char *conserved_motifs = (char *)malloc((end - begin) * sizeof(char)+1);
            int k=0;
            for (j = begin; j < end; j++)
            {
                conserved_motifs[k] = *(conserved + j);
                k++;
                //printf("%c", *(conserved + j));
            }
            conserved_motifs[k] = '\0';
            strcat(stock_motifs,conserved_motifs);
            char *s_score[10];
            sprintf(s_score," %8.2f",score);
            strcat(stock_motifs,":");
            strcat(stock_motifs,s_score);
            strcat(stock_motifs,"\n\n");
            // printf("  \t:%8.2f\n", score);
            // printf("\n");

            free(conserved_motifs);

#endif
        }
    }
    free(stock_motifs);
    return firstmotif;
}
/*****************************************************************/
