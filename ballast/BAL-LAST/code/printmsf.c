/*** printmsf ****************************************************/
/*                                                               */
/*   Prints MSF file with identified motifs for those sequences  */
/*   with Expect value below threshold.                          */
/*   No return value.                                            */
/*                                                               */
/*****************************************************************/
/* SeqHSP *first        : first sequence producing HSPs          */
/* float   threshold    : Expect threshold                       */
/* FILE   *outfile      : output file                            */
/*                                                               */
/* SeqHSP *seqres       : current sequence producing HSPs        */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "types.h"

#define GCG_LINELENGTH 50


int SeqGCGCheckSum(char *seq, int len);



void    printmsf (char *conserved, int length, SeqHSP *first, float threshold, FILE *outfile)
{
SeqHSP *seqres ;

char line[50], *ptrstr,residue;
int i,block,chunks;
int pos1, pos2;
long grand_checksum;
long checksum;


#ifdef DEBUG
for (i=0;i<length;i+=50)
	{
	strncpy (line,(char *)(conserved+i),50);
	line[50]='\0';
	printf ("Query           %s\n", line);

	for (seqres = first; seqres != NULL ; seqres=seqres->next)
		{
		if ((seqres->type=='p') && (seqres->p < threshold) && (seqres->aligned != NULL))
			{
			strncpy (line,(char *)(seqres->aligned+i),50);
			line[50]='\0';
			printf ("%-15s %s\n",seqres->name, line);
			}
		}
	printf ("\n\n");
	}
#endif


checksum = SeqGCGCheckSum(conserved, length);
grand_checksum = checksum;

for (seqres = first; seqres != NULL ; seqres=seqres->next)
	{
	if ((seqres->type=='p') && (seqres->p < threshold) && (seqres->aligned != NULL))
		{
		seqres->check = SeqGCGCheckSum(seqres->aligned, length);
		grand_checksum +=  seqres->check;
		}
	}

grand_checksum = grand_checksum % 10000;

fprintf (outfile, "PileUp\n\n");
fprintf (outfile, "\n\n   MSF:%5d  Type: P",length);

fprintf (outfile,"    Check:%6ld   ..\n\n", (long) grand_checksum);

fprintf(outfile," Name: Query oo  Len:%5d  Check:%6ld  Weight:  1.00\n",length,checksum);

for (seqres = first; seqres != NULL ; seqres=seqres->next)
	{
	if ((seqres->type=='p') && (seqres->p < threshold) && (seqres->aligned != NULL))
		{
		fprintf(outfile," Name: %s oo  Len:%5d  Check:%6ld  Weight:  1.00\n",seqres->name,length,seqres->check);
		}
	}

fprintf (outfile, "\n//\n");

chunks = length/GCG_LINELENGTH;
if(length % GCG_LINELENGTH != 0) ++chunks;

for(block=1; block<=chunks; block++) 
		{
                fprintf(outfile,"\n\n");
                pos1 = ((block-1) * GCG_LINELENGTH) + 1;
                pos2 = (length<pos1+GCG_LINELENGTH-1)? length : pos1+GCG_LINELENGTH-1;

		fprintf(outfile,"\nQuery           ");
		for(i=pos1; i<=pos2; i++)
			{
			fprintf(outfile,"%c", *(conserved+i-1));
			if(i % 10 == 0) fprintf(outfile," ");
			}

                for(seqres = first; seqres != NULL ; seqres=seqres->next)
			{
			if ((seqres->type=='p') && (seqres->p < threshold) && (seqres->aligned != NULL))
				{
				fprintf(outfile,"\n%-15s ",seqres->name);
				for(i=pos1; i<=pos2; i++) 
					{
					fprintf(outfile,"%c", *(seqres->aligned+i-1));
					if(i % 10 == 0) fprintf(outfile," ");
					}
				}
			}
		}

}
