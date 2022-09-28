/**
 * @file loadhspv2.c
 * @author Ihsane E.
 * @brief 
 * @version 0.1
 * @date 2022-09-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/*** loadHSP *****************************************************/
/*                                                               */
/*   Loads HSPs into seqres structure and derives contribution   */
/*   to profile and similarity profile for rescoring.            */
/*   Returns contribution to profile pointer.                    */
/*                                                               */
/*****************************************************************/
/* SeqHSP *seqres    : Current result sequence structure         */
/* FILE   *file      : Current BLAST result file                 */
/* char   *line      : Current line                              */
/* int    *length    : Query sequence (i.e. profile) length      */
/* char   *conserved : Residues of query sequences appearing in  */
/*                     at least one HSP                          */
/* char   *type      : 'p' for protein or 'n' for nucleotide     */
/*                                                               */
/* double *profil    : contribution to profile                   */
/* double *ptr       : pointer to profil                         */
/* double *simptr    : current similarity profile                */
/* SimPrf *simprf    : whole HSP description                     */
/* int     debut     : start position of current HSP             */
/* int     begin     : start position of currently read portion  */
/*                     of HSP                                    */
/* int     end       : end position of current HSP               */
/* int     debdb     : start position of current HSP in DB seq.  */
/* int     begdb     : start position of currently read portion  */
/*                     of HSP in DB sequence                     */
/* int     enddb     : end position of current HSP in DB sequence*/
/* int     dline     : position in line where sequence actually  */
/*                     starts in BLAST result files              */
/* int     ok        : 0 if first part of HSP not yet read,      */
/*                     1 otherwise                               */
/* int     okhsp     : 0 if first part of database sequence not  */
/*                       yet read                                */
/*                     1 otherwise                               */
/* double  facteur   : modifying factor for contribution to      */
/*                     profile = 1 - P(N)                        */
/* int     fctr      : factor to build similarity profile        */
/* char   *seq       : HSP alignment description                 */
/* char   *seqhsp    : database sequence in HSP                  */
/* char   *outtext   : last part of database sequence description*/
/* double  p         : read E(N) value                           */
/*                     sequence into profile                     */
/* int     identique : 0 if 100% identity, 1 otherwise           */
/* int     taux      : % identity for HSP                        */
/* int     naas      : number of amino acid in 100% identical HSP*/
/*****************************************************************/


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
	/**********déclaration des variables***********/
	double *profil, *ptr, *simptr;
	SimPrf *simprf;

	int i, debut, begin, end;
	int debdb, begdb, enddb;
	int dline; //position in line where sequence actually starts in BLAST result files  
	int ok = 0;
	int okhsp = 0;
	int endofdbseq = 0;
	int gapped = 0;
	double facteur;
	int fctr;
	char *seq = NULL;
	char *queryseq = NULL;
	char *seqhsp = NULL;
	char *outtext=NULL;
	double p;

	char *ptrstr;
	int identique = 1, taux, naas;

	char *begline;

	int n;
	double maxp;

	int dline1, dline2;
	char *startline;
// concerne les paramètres lors de l'execution du programme
	if (getargdouble("-maxp", &maxp) == NULL)
		maxp = DEFMAXP;
	if (type == 'n')
	{
		if (getargdouble("-nmaxp", &maxp) == NULL)
			maxp = DEFMAXP;
	}
	//on initialise le seqres
	seqres->sc = 0;
	seqres->nmatch = 0;
	seqres->type = type;
	seqres->risedup = ' ';	
	seqres->sim = (SimPrf *)malloc(sizeof(SimPrf));
	simprf = seqres->sim;
	simprf->text = NULL;
	simprf->score = 0;
	simprf->maxscore = 0;
	simprf->nmatch = 0;
	//on alloue de la mémoire au profil
	profil = (double *)malloc(sizeof(double) * length); //pb ici

	for (i = 0; i < length; i++)
	{
		ptr = (double *)(profil + i);//typecasting double adresse de profil+i
		*ptr = 0; //valeur de p de base mais changer en 0
	}
	seqres->p = 1;
	seqres->prob = maxp;

	outtext = (char *)malloc(strlen(line) + 1);
	strcpy(outtext, line);

	if (line[0] == '>')
	{
		strcpy(line, &line[1]);
	}

	if (strlen(line) <= 90)
	{
		for (i = strlen(line) - 1; i < 90; i++)
		{
			line[i] = ' ';
		}
	}

	line[90] = '\0';
	

	seqres->desc = (char *)malloc(strlen(line)+1 );
	strcpy(seqres->desc, line);

	ptrstr = strtok(line, " ");
	seqres->name = (char *)malloc(strlen(ptrstr) + 1);
	strcpy(seqres->name, ptrstr);
	//fprintf(stdout, "seqres->name est :%s\n", seqres->name);

	ptrstr = strchr(seqres->name, ':');
	if (ptrstr != NULL){
		*ptrstr = '|';
	}

	ptrstr = strtok(NULL, " ");
	if (ptrstr != NULL)
	{
		seqres->access = (char *)malloc(strlen(ptrstr) + 1);
		strcpy(seqres->access, ptrstr);
	}
	else
	{
		seqres->access = (char *)malloc(strlen(seqres->name) + 1);
		strcpy(seqres->access, seqres->name);
	}

	fgets(line, 256, file);

	begline = line;
	if (*begline == ' ')
		begline++;

	/* begline = (char *) (line + strcspn (line," ") + 1);*/

	while (strncmp(begline, "Score", 5) != 0)
	{
		outtext = (char *)malloc(strlen(outtext) + strlen(line) + 1);
		strcpy(outtext, line); //au lieu de strcat

		fgets(line, 256, file);
		begline = line;
		if (*begline == ' ')
			begline++;

		/*begline = (char *) (line + strcspn (line," ") + 1);*/
	}
	seqres->outtext = outtext;
	
	/**** Read 1st HSP probability as given by BLAST **********************************/
	ptrstr = (char *)(strstr(line, "= e-"));
	if (ptrstr != NULL)
	{
		ptrstr = (char *)(ptrstr + 1);
		*ptrstr = '1';
	}
	sscanf((char *)(strrchr(line, '=') + 1), "%lf", &p);
	seqres->prob = p;

	simprf->p = p;
	if ((p >= maxp) || (p > 1))
		p = 1;
	facteur = (1.0 - p);
	fctr = 1;
	simprf->text = (char *)malloc(strlen(line) + 1);
	strcpy(simprf->text, line);
	/**********************************************************************************/

	while (endofdbseq == 0)
	{
		/**** Huuhhhh.... t'looks like we've run off the file and should stop *****/
		if (!fgets(line, 256, file) || (feof(file) != 0))
		{
			line[0] = '\0';
			endofdbseq = 1;
		}
		/**** This is supposed to be the end of the BLAST results, we don't    ****/
		/**** need to read the file any further and may leave                  ****/

		if ((strncmp(line, "WARNING:", 8) == 0) || (strncmp(line, "  Database:", 11) == 0) || (strncmp(line, "Parameters:", 11) == 0))
		{
			line[0] = '\0';
			endofdbseq = 1;
			simprf->next = NULL;
		}
		/**** The current line starts with "Query", we are therefore reading  *****/
		/**** the alignment into *seq                                         *****/

		if (strncmp(line, "Query", 5) == 0)
		{

			sscanf((char *)(strpbrk(line, "0123456789")), "%d", &debut);
			sscanf((char *)(strrchr(line, ' ')), "%d", &end);
			dline = strlen(line) - strlen(strpbrk(line + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ-"));

			ptrstr = (char *)(strrchr(line, ' '));
			*ptrstr = '\0';
			//memcpy(line, (char*)(line+dline), strlen(line)+dline);//
			strcpy(line, (char *)(line + dline));
			
			if (ok == 0)
			{		
				queryseq = (char *)malloc(strlen(line) + 1);
				*queryseq = '\0';
			}
			else
			{
				queryseq = (char *)realloc(queryseq,strlen(line) + strlen(queryseq) + 1);//it was 1
			}
			strcat(queryseq, line);//au lieu de strcat 

			fgets(line, 256, file);

			line[strlen(line) - 1] = '\0';
			n = (end - debut + 1) - strlen((char *)(line + dline));
			if (n > 0)
				strncat(line, "                                                            ", n);

			if (ok == 0)
			{
				begin = debut;
				seq =(char *)malloc(strlen(line)*strlen(line));	
				*seq = '\0';
			}
			else
			{
				seq = (char *)realloc(seq, strlen(line) + strlen(seq) + 1);
			}

			strcat(seq, (char *)(line + dline));//strcat ici de base

			ok = 1;
		}

		
		/**** The current line starts with "Sbjct", we are therefore reading  *****/
		/**** the database sequence into seqhsp                               *****/

		if (strncmp(line, "Sbjct", 5) == 0)
		{
			sscanf((char *)(strpbrk(line, "0123456789")), "%d", &debdb);
			sscanf((char *)(strrchr(line, ' ')), "%d", &enddb);

			dline1 = strlen(line);
			startline = strpbrk(line + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ-");
			if (startline == NULL) /* Julie : added to avoid crash if HSP is badly formatted */
			{
				line[0] = '\0';
				endofdbseq = 1;
			}
			else
			{
				dline2 = strlen(startline);
				dline = dline1 - dline2;
				ptrstr = (char *)strchr((char *)(line + dline), ' ');
				if (ptrstr != NULL)
					*ptrstr = '\0';

				if (okhsp == 0)
				{
					begdb = debdb;
					seqhsp = (char *)malloc(strlen(line) + 1);
					*seqhsp = '\0';
					simprf->sens = '+';
					if (debdb > enddb)
						simprf->sens = '-';
				}
				else
				{
					seqhsp = (char *)realloc(seqhsp, strlen(line) + strlen(seqhsp) + 1);
				}

				strcat(seqhsp, (char *)(line + dline));
				okhsp = 1;
			}
		}

		/**** Don't keep HSPs longer than a tenth of the query sequence length ****/
		/**** AND presenting a 100% identity                                   ****/

		if (strncmp(line, " Identities", 11) == 0)
		{
			if (strstr(line, "Gaps"))
			{
				gapped = 1;
			}
			else
			{
				gapped = 0;
			}
			simprf->text = (char *)realloc(simprf->text, strlen(simprf->text) + strlen(line) + 14);
			strcat(simprf->text, "             ");
			strcat(simprf->text, line);
			sscanf((char *)(strchr(line, '(') + 1), "%d", &taux);
			sscanf((char *)(strchr(line, '=') + 1), "%d", &naas);
			simprf->pcid = taux;

			simprf->nid = naas;

		}
		/**** At this point, we have finished reading the current HSP and are  ****/
		/**** ready to deal with it                                            ****/

		if (((strchr("[1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz>", *line)) && ((strncmp(line, "Query", 5) != 0) && (strncmp(line, "Sbjct", 5) != 0) && (strncmp(line, "Score", 5) != 0) && (strncmp(line, "Identities", 10) != 0))) || (strncmp(line, "WARNING:", 8) == 0) || (strncmp(line, "  Database:", 11) == 0) || (strncmp(line, "Score", 5) == 0) || (strncmp(line, " Score", 6) == 0) || (strncmp(line, "Parameters:", 11) == 0))
		{
			/**** Let's check that the current database sequence doesn't contain   ****/
			/**** too many low complexity subsequences in the region where the HSP ****/
			/**** has been found. Otherwise, we simply reject this HSP (p set to 1)****/

			if (filter(seqhsp, seq) == 0)
			{
				p = 1;
			}

			simprf->hsp = seqhsp;
			simprf->queryseq = queryseq;
			simprf->aln = seq;
			simprf->next = NULL;
			queryseq = NULL;
			seq = NULL;

			if ((p >= maxp) || (p > 1))
				p = 1;
			if (p < seqres->p)
			{
				seqres->p = p;
			}

			simprf->begin = begin - 1;
			simprf->end = end - 1;
			
			simprf->begdb = begdb - 1;
			simprf->enddb = enddb - 1;
		
			if (simprf->begdb > simprf->enddb)
			{
				simprf->begdb = -simprf->begdb;
				simprf->enddb = -simprf->enddb;
			}

			/**************************************************************************/

			if (gapped){
				simprf = handlegaps(simprf);
			}
			/**** The current line starts with " Score", we are therefore starting ****/
			/**** to read a new HSP                                                ****/

			begline = line;
			if (*begline == ' ')
				begline++;

			/*begline = (char *) (line + strcspn (line," ") + 1);	*/

			if (strncmp(begline, "Score", 5) == 0)
			{
				if (ok == 1)
				{
					simprf->next = (SimPrf *)malloc(sizeof(SimPrf));
					simprf = simprf->next;
					simprf->text = (char *)malloc(strlen(line) + 1);
					strcpy(simprf->text, line);
					ok = 0;
					okhsp = 0;
					simprf->score = 0;
					simprf->maxscore = 0;
					simprf->nmatch = 0;
				}
				ptrstr = (char *)(strstr(line, "= e-"));
				if (ptrstr != NULL)
				{
					ptrstr = (char *)(ptrstr + 1);
					*ptrstr = '1';
				}
				sscanf((char *)(strrchr(line, '=') + 1), "%lf", &p);
				if (p < seqres->prob)
					seqres->prob = p;
				simprf->p = p;
			}
			if (*line == '>')
				endofdbseq = 1;

		}


	}



#ifdef DEBUG
	printf("%s\n", seqres->desc);
#endif

	for(simprf = seqres->sim; simprf != NULL; simprf = simprf->next)
	{
		seqhsp = simprf->hsp;
		seq = simprf->aln;
		begin = simprf->begin + 1;
		end = simprf->end + 1;
		begdb = simprf->begdb + 1;
		enddb = simprf->enddb + 1;

		simprf->prf = (double *)malloc(sizeof(double) * (end - begin + 1));

		p = simprf->p;
		if ((p >= maxp) || (p > 1)){
			p = 1;
		}
		facteur = (1 - p);
		fctr = 1;


#ifdef DEBUG
		printf("Query %6d %s %d\n", begin, simprf->queryseq, end);
		printf("             %s\n", seq);
		printf("Sbjct %6d %s %d\n\n\n", begdb, seqhsp, enddb);
		fflush(NULL);
#endif
		/**** Update the profile for this sequence accounting for the current  ****/
		/**** HSP and create the similarity profile for the current HSP        ****/
		for (i = begin - 1; i < end; i++)
		{
			ptr = (double *)(maxprofile + i);
			if ((taux == 100) && (naas > (length / 10))){
				identique = 0;
			}
			*ptr += facteur * identique;
			
			ptr = (double *)(profil + i);
			simptr = (double *)(simprf->prf + i - begin + 1);
			*simptr = 0.0;
			/*** Aligned Aas in Query sequence and Database sequence are ****/
			/*** identical                                               ****/

			if ((*(seq + i - begin + 1) != '+') && (*(seq + i - begin + 1) != ' ') && (*(seq + i - begin + 1) != 'x'))
			{
				*ptr = facteur * identique;
				*simptr = ID * fctr;
				ptrstr = (char *)(conserved + i);
				*ptrstr = *(seq + i - begin + 1);
				//printf("on est identique\n");
			}
			else
			{
				/*** Aligned Aas in Query and Database sequences are  ***/
				/*** similar                                          ***/
				if (*(seq + i - begin + 1) == '+')
				{
					//printf("similarité\n");

					*ptr = identique * facteur / 2; //ici pb
					*simptr = SIM * fctr;
				}
				else
				{
					/*** Well... actually they're different       ***/
					//printf("différent\n");
					*ptr = identique * NETRA * facteur; //pb ici
					*simptr = RIEN * fctr;
				}
			}
		}
			
	}
	fprintf(stdout, "le smptr est de : %lf\n", *simptr);

	/**** Bye, bye...  ****************************************************************/

	return profil;
}
/**********************************************************************************/
