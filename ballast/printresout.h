/*** printresout *************************************************/
/*                                                               */
/*   Writes rescored and re-ordered sequences and HSPs to        */
/*   result file.                                                */
/*   No return value.                                            */
/*                                                               */
/*****************************************************************/
/* SeqHSP *first        : first sequence producing HSPs          */
/* FILE   *outfile      : output file                            */
/*                                                               */
/* SeqHSP *seqres       : current sequence producing HSPs        */
/* char    shortdesc[50]: short description of the sequence      */
/* float   threshold    : threshold for output                   */
/*****************************************************************/

#define BLOCKLEN 60

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readargs.h"
#include "macros.h"
#include "types.h"

void    printnicedouble (FILE *outfile, double p);

void    printresout (SeqHSP *first, FILE *outfile, double maxscore)
{
SeqHSP *seqres;
SimPrf *s,*s1,*s2;
char shortdesc[50];
char queryseq[BLOCKLEN+1],hsp[BLOCKLEN+1],aln[BLOCKLEN+1];
char *maskfilename;
float threshold;
double score,relscore;
int begin,end,begdb,enddb;
int i=0;
int mask=0;

/*
fprintf (outfile,"\n\nSequences producing High-scoring Segment Pairs:                 Score   P(N)          r [blast r]\n\n");
*/
fprintf (outfile,"\n\nSequences producing High-scoring Segment Pairs:                 rank [blast rank]   Score         n   E(N)\n\n");

if (getargfloat ("-t",&threshold) == NULL) threshold = 0.0;
if (getargchar ("-mask",&maskfilename) != NULL) mask=1;


/*****************************************************************/
/*** For every sequence producing HSPs with a score above        */
/*** threshold write one line in the list                        */
/*****************************************************************/

seqres = first;

while ((seqres!=NULL) && ( ((mask == 0) && (seqres->prob <= ELIMIT)) || (seqres->sc > threshold) ) )
        {
        /*********************************************************/
        /*** Current sequence is the first or doesn't have the ***/
        /*** same score as the previous one                    ***/
        /*********************************************************/


        if ((seqres == first) || (seqres->sortedprev->sc != seqres->sc))
                {
                if ((mask == 0) && (seqres->risedup == '^') && ((seqres->sortedprev == NULL) || (seqres->sortedprev->risedup != '^')))
                        {
fprintf (outfile,"-------------------------------------------------------------------------------------------------------\n");
                        }
                fprintf (outfile,"%s ",seqres->desc);
        /*
                fprintf (outfile,"%s %8.2f ",seqres->desc,seqres->sc);
                printnicedouble(outfile,seqres->prob);
        */


                /*************************************************/
                /*** If current sequence was included in       ***/
                /*** 'profile' then add a '*'                  ***/
                /*************************************************/

                if (seqres->p != 1)
                        {
                        fprintf (outfile,"*%c",seqres->risedup);
                        }
                else
                        {
                        fprintf (outfile," %c",seqres->risedup);
                        }
                /*************************************************/

                fprintf (outfile," %4d [%4d] ",++i,seqres->rank);

                fprintf (outfile," %8.2f (%5.1f%%) %3d ",seqres->sc,100*seqres->sc/maxscore,seqres->nmatch);
                printnicedouble(outfile,seqres->prob);
                fprintf (outfile,"\n");

/*
                if ((mask == 0) && (seqres->prob <= ELIMIT) && (seqres->sortednext->prob > ELIMIT))
                        {
fprintf (outfile,"-------------------------------------------------------------------------------------------------------\n");
                        }
*/
                }
        /*********************************************************/

        else

        /*********************************************************/
        /*** Current sequence has the same score as the previous */
        /*** one.                                              ***/
        /*********************************************************/

                {
                fprintf (outfile,"%s ",seqres->desc);


                /*************************************************/
                /*** If current sequence was included in       ***/
                /*** 'profile' then add a '*'                  ***/
                /*************************************************/

                if (seqres->p != 1)
                        {
                        fprintf (outfile,"*%c",seqres->risedup);
                        }
                else
                        {
                        fprintf (outfile," %c",seqres->risedup);
                        }
                /*************************************************/


                fprintf (outfile," %4d [%4d] ",++i,seqres->rank);
                fprintf (outfile,"     \"        \"    %3d ",seqres->nmatch);
                printnicedouble(outfile,seqres->prob);
                fprintf (outfile,"\n");
                }
        /*********************************************************/


        /*********************************************************/
        /*** Writes twin sequences, i.e. sequences producing   ***/
        /*** exactly the same HSPs though not necessarily with ***/
        /*** the same P(N) nor Blast score                     ***/
        /*********************************************************/

        if (seqres->twins != NULL) fprintf (outfile,"%s",seqres->twins);

        /*********************************************************/
        if ((mask == 0) && (seqres->prob <= ELIMIT) && ((seqres->sortednext == NULL) || (seqres->sortednext->prob > ELIMIT)))
                {
fprintf (outfile,"-------------------------------------------------------------------------------------------------------\n");
                }

        seqres=seqres->sortednext;
        }
/*****************************************************************/

fprintf (outfile,"\n\n");

s = (SimPrf *) malloc (sizeof(SimPrf));



/*****************************************************************/
/*** For every sequence producing HSPs with a score above        */
/*** threshold write corresponding HSPs sorted by position       */
/*****************************************************************/

seqres = first;

while ((seqres!=NULL) && ( ((mask == 0) && (seqres->prob <= ELIMIT)) || (seqres->sc > threshold) ) )
        {
        /*********************************************************/
        /*** Sort HSPs by position (begin)                     ***/
        /*********************************************************/

        if (getargbool("-nosort")==FALSE)
                {
                for (s1=seqres->sim;s1->next!=NULL;s1=s1->next)
                        {
                        for (s2=s1->next;s2!=NULL;s2=s2->next)
                                {
                                if (s1->begin > s2->begin)
                                        {
                                        s->text   = s1->text;
                                        s->score  = s1->score;
                                        s->p         = s1->p;
                                        s->maxscore  = s1->maxscore;
                                        s->nmatch = s1->nmatch;
                                        s->begin  = s1->begin;
                                        s->end    = s1->end;
                                        s->begdb  = s1->begdb;
                                        s->enddb  = s1->enddb;
                                        s->prf    = s1->prf;
                                        s->sens   = s1->sens;
                                        s->queryseq = s1->queryseq;
                                        s->hsp  = s1->hsp;
                                        s->aln  = s1->aln;

                                        s1->text  = s2->text;
                                        s1->score  = s2->score;
                                        s1->p         = s2->p;
                                        s1->maxscore  = s2->maxscore;
                                        s1->nmatch = s2->nmatch;
                                        s1->begin = s2->begin;
                                        s1->end   = s2->end;
                                        s1->begdb = s2->begdb;
                                        s1->enddb = s2->enddb;
                                        s1->prf   = s2->prf;
                                        s1->sens   = s2->sens;
                                        s1->queryseq    = s2->queryseq;
                                        s1->hsp = s2->hsp;
                                        s1->aln = s2->aln;

                                        s2->text  = s->text;
                                        s2->score  = s->score;
                                        s2->p         = s->p;
                                        s2->maxscore  = s->maxscore;
                                        s2->nmatch = s->nmatch;
                                        s2->begin = s->begin;
                                        s2->end   = s->end;
                                        s2->begdb = s->begdb;
                                        s2->enddb = s->enddb;
                                        s2->prf   = s->prf;
                                        s2->sens   = s->sens;
                                        s2->queryseq    = s->queryseq;
                                        s2->hsp  = s->hsp;
                                        s2->aln  = s->aln;
                                        }
                                }
                        }
                }
        /*********************************************************/


        /*********************************************************/
        /*** Write description of the current sequence         ***/
        /*********************************************************/

        fprintf (outfile,"%s",seqres->outtext);

        fprintf (outfile,"Total Ballast Score = %8.2f (%5.1f%%) for %3d overlapping word(s)\n\n\n",seqres->sc,100*seqres->sc/maxscore,seqres->nmatch);

        /*********************************************************/


        /*********************************************************/
        /*** Write corresponding twin sequences                ***/
        /*********************************************************/

        if (seqres->twins != NULL)
                {
                fprintf (outfile,"  Other sequences producing the same alignments :\n");
                fprintf (outfile,"%s\n",seqres->twins);
                }

        /*********************************************************/


        /*********************************************************/
        /*** Write HSPs                                        ***/
        /*********************************************************/

        for (s1=seqres->sim;s1!=NULL;s1=s1->next)
                {
                if (((mask == 0) && (s1->p <= ELIMIT) && (seqres->sc == 0)) || (s1->score > 0) )
                        {
                        if (s1->maxscore > 0)
                                {
                                relscore = (100.0 * s1->score)/s1->maxscore;
                                }
                        else
                                {
                                relscore = 0.0;
                                }
                        fprintf (outfile," Ballast score = %8.2f (%5.1f%%), %d word(s)",s1->score,relscore,s1->nmatch);
                        fprintf (outfile , " << %c >>  \n original HSP Blast", s1->sens);
                        fprintf (outfile,"%s\n",s1->text);
/*
                        strncpy(queryseq,s1->queryseq,BLOCKLEN);
                        queryseq[BLOCKLEN]='\0';
                        strncpy(hsp,s1->hsp,BLOCKLEN);
                        hsp[BLOCKLEN]='\0';
                        strncpy(aln,s1->aln,BLOCKLEN);
                        aln[BLOCKLEN]='\0';
                        begin = s1->begin + 1;
                        begdb = s1->begdb + 1;
                        end   = begin + strlen (queryseq) - 1;
                        enddb = begdb + strlen (hsp) - 1;
                        fprintf (outfile,"Query:%6d %s %d\n",begin,queryseq,end);
                        fprintf (outfile,"             %s\n",aln);
                        fprintf (outfile,"Sbjct:%6d %s %d\n\n",begdb,hsp,enddb);
*/
                        for (i=0;i< (strlen(s1->queryseq)/BLOCKLEN);i++)
                                {
                                strncpy(queryseq,(char *)(s1->queryseq+i*BLOCKLEN),BLOCKLEN);
                                queryseq[BLOCKLEN]='\0';
                                strncpy(hsp,(char *)(s1->hsp+i*BLOCKLEN),BLOCKLEN);
                                hsp[BLOCKLEN]='\0';
                                strncpy(aln,(char *)(s1->aln+i*BLOCKLEN),BLOCKLEN);
                                aln[BLOCKLEN]='\0';
                                begin = s1->begin + 1 + i*BLOCKLEN;
                                begdb = s1->begdb + 1 + i*BLOCKLEN;
                                end   = begin + strlen (queryseq) - 1;
                                enddb = begdb + strlen (hsp) - 1;
                                fprintf (outfile,"Query:%6d %s %d\n",begin,queryseq,end);
                                fprintf (outfile,"             %s\n",aln);
                                fprintf (outfile,"Sbjct:%6d %s %d\n\n",begdb,hsp,enddb);
                                }
                        strncpy(queryseq,(char *)(s1->queryseq+i*BLOCKLEN),BLOCKLEN);
                        strncpy(hsp,(char *)(s1->hsp+i*BLOCKLEN),BLOCKLEN);
                        strncpy(aln,(char *)(s1->aln+i*BLOCKLEN),BLOCKLEN);
                        if (*queryseq != '\0')
                                {
                                begin = s1->begin + 1 + i*BLOCKLEN;
                                begdb = s1->begdb + 1 + i*BLOCKLEN;
                                end   = begin + strlen (queryseq) - 1;
                                enddb = begdb + strlen (hsp) - 1;
                                fprintf (outfile,"Query:%6d %s %d\n",begin,queryseq,end);
                                fprintf (outfile,"             %s\n",aln);
                                fprintf (outfile,"Sbjct:%6d %s %d\n\n",begdb,hsp,enddb);
                                }
                        fprintf (outfile,"\n");
                        }
                }
                fprintf (outfile,"\n\n");

        /*********************************************************/

        seqres=seqres->sortednext;
        }
/*****************************************************************/
return ;
}