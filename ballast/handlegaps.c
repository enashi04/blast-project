/*** handlegaps **************************************************/
/*                                                               */
/*   Ungaps gapped HSPs from Gapped Blast results                */ 
/*   returns last ungapped HSP portion                           */
/*                                                               */
/*****************************************************************/
/* char   *queryseq     : Query sequence                         */ 
/* char   *aln          : Blast alignment middle line            */
/* char   *seqhsp       : HSP sequence                           */ 
/*                                                               */
/*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

/*****************************************************************/

SimPrf *handlegaps (SimPrf *simprf)
{
char *queryseq;
char *aln;
char *seqhsp;
char *querynext;
char *seqhspnext;
char *ptr;
int   gaplen, qnogap, dnogap;

queryseq = simprf->queryseq;
aln      = simprf->aln;
seqhsp   = simprf->hsp;

querynext  = strchr (queryseq,'-');
seqhspnext = strchr (seqhsp,'-');

qnogap = strcspn (queryseq,"-");
dnogap = strcspn (seqhsp,"-");

#ifdef DEBUG
printf ("%s\n",simprf->text);
printf ("%s\n%s\n",queryseq,seqhsp);
#endif

while (querynext || seqhspnext) 
	{
	if (querynext && (qnogap <= dnogap))
		{
		simprf->end = simprf->begin + qnogap - 1;
		simprf->enddb = simprf->begdb + qnogap - 1;
		gaplen     = strspn (querynext,"-");
		simprf->next = (SimPrf *) malloc (sizeof (SimPrf));
		simprf->next->text = simprf->text;
		simprf->next->begin = simprf->end + 1;
		simprf->next->begdb = simprf->enddb + gaplen + 1;
		simprf->next->p    = simprf->p;
		simprf->next->pcid= simprf->pcid;
		simprf->next->nid = simprf->nid;
		simprf->next->sens = simprf->sens;
		
		simprf = simprf->next;

		simprf->score = 0;
		simprf->maxscore = 0;
		simprf->nmatch= 0;

		*querynext = '\0';
		ptr        = (char *) (seqhsp + qnogap);
		*ptr = '\0';
		simprf->hsp     = (char *) (ptr + gaplen);
		ptr        = (char *) (aln + qnogap);
		*ptr = '\0';
		simprf->aln        = (char *) (ptr + gaplen);
		simprf->queryseq   = (char *) (querynext + gaplen);
		}
	else if (seqhspnext)
		{
		simprf->end = simprf->begin + dnogap - 1;
		simprf->enddb = simprf->begdb + dnogap - 1;
		gaplen     = strspn (seqhspnext,"-");
		simprf->next = (SimPrf *) malloc (sizeof (SimPrf));
		simprf->next->text = simprf->text;
		simprf->next->begin = simprf->end + gaplen + 1;
		simprf->next->begdb = simprf->enddb + 1;
		simprf->next->p    = simprf->p;
		simprf->next->pcid= simprf->pcid;
		simprf->next->nid = simprf->nid;
		simprf->next->sens = simprf->sens;
		
		simprf = simprf->next;

		simprf->score = 0;
		simprf->maxscore = 0;
		simprf->nmatch= 0;

		*seqhspnext= '\0';
		ptr        = (char *) (queryseq + dnogap);
		*ptr = '\0';
		simprf->queryseq   = (char *) (ptr + gaplen);
		ptr        = (char *) (aln + dnogap);
		*ptr = '\0';
		simprf->aln        = (char *) (ptr + gaplen);
		simprf->hsp     = (char *) (seqhspnext + gaplen);
		}
#ifdef DEBUG
	printf ("Query: %s\n       %s\nSbjct: %s\n\n",queryseq,aln,seqhsp);
#endif

	queryseq = simprf->queryseq;
	aln      = simprf->aln;
	seqhsp   = simprf->hsp;

	querynext  = strchr (queryseq,'-');
	seqhspnext = strchr (seqhsp,'-');

	qnogap = strcspn (queryseq,"-");
	dnogap = strcspn (seqhsp,"-");
#ifdef DEBUG
	printf ("gaplen=%d; qnogap=%d; dnogap=%d\n",gaplen,qnogap,dnogap);
#endif
	}
#ifdef DEBUG
printf ("Query: %s\n       %s\nSbjct: %s\n\n",queryseq,aln,seqhsp);
#endif

simprf->end = simprf->begin + qnogap - 1;
simprf->enddb = simprf->begdb + qnogap - 1;
simprf->next = NULL;

return simprf;
}
/****************************************************************************/
