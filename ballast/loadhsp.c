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

	int i, j, debut, begin, end;
	int debdb, begdb, enddb;
	int dline; //position in line where sequence actually starts in BLAST result files  
	int k = 1;
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

	
	/**** Initialisation **************************************************************/
// concerne les paramètres lors de l'execution du programme
	if (getargdouble("-maxp", &maxp) == NULL)
		maxp = DEFMAXP;
	if (type == 'n')
	{
		if (getargdouble("-nmaxp", &maxp) == NULL)
			maxp = DEFMAXP;
	}
//on initialise le résultat de la séquence 
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
	profil = (double *)malloc(sizeof(double) * length);

	for (i = 0; i < length; i++)
	{
		ptr = (double *)(profil + i);//typecasting double adresse de profil+i
		*ptr = 0; //valeur de p de base mais changer en 0
		//printf("ptr : %d\n", ptr);
	}
	seqres->p = 1;
	seqres->prob = maxp;

	outtext = (char *)realloc(outtext, strlen(line) + 1);
	strcpy(outtext, line);

	if (line[0] == '>')
	{
		strcpy(line, &line[1]);
	}

	if (strlen(line) <= 80)
	{
		for (i = strlen(line) - 1; i < 80; i++)
		{
			line[i] = ' ';
		}
	}

	line[80] = '\0';

	seqres->desc = (char *)malloc(strlen(line)+1 );
	strcpy(seqres->desc, line);
	fprintf(stdout, "seqres->desc est :%s\n", seqres->desc);

	ptrstr = strtok(line, " ");
	seqres->name = (char *)malloc(strlen(ptrstr) + 1);
	strcpy(seqres->name, ptrstr);
	fprintf(stdout, "seqres->name est :%s\n", seqres->name);

	ptrstr = strchr(seqres->name, ':');
	if (ptrstr != NULL){
		*ptrstr = '|';
	}

	ptrstr = strtok(NULL, " ");
	if (ptrstr != NULL)
	{
		seqres->access = (char *)malloc(strlen(ptrstr) + 1);
		strcpy(seqres->access, ptrstr);
		//fprintf(stdout, "seqres->access est :%s\n", seqres->access);
	}
	else
	{
		seqres->access = (char *)malloc(strlen(seqres->name) + 1);
		strcpy(seqres->access, seqres->name);
		//fprintf(stdout, "seqres->access est :%s\n", seqres->access);
	}

	fgets(line, 256, file);

	begline = line;
	if (*begline == ' ')
		begline++;
		//fprintf(stdout, "begline %s\n", begline);

	/* begline = (char *) (line + strcspn (line," ") + 1);*/

	while (strncmp(begline, "Score", 5) != 0)
	{
		outtext = (char *)malloc(strlen(outtext) + strlen(line) + 1);
		strcat(outtext, line);
		fgets(line, 256, file);
		begline = line;
		if (*begline == ' ')
			begline++;

		/*begline = (char *) (line + strcspn (line," ") + 1);*/
	}
	seqres->outtext = outtext;

	// fprintf(stdout, "outtext est :%s\n", seqres->outtext);

	/**********************************************************************************/

	/**********************************************************************************/
	/**** Read 1st HSP probability as given by BLAST **********************************/
	// fprintf(stdout, "ici on va lire la premiere probabilité de hsp donnée par blast\n");
	ptrstr = (char *)(strstr(line, "= e-"));
	if (ptrstr != NULL)
	{
		ptrstr = (char *)(ptrstr + 1);
		*ptrstr = '1';
	}
	sscanf((char *)(strrchr(line, '=') + 1), "%lf", &p);
	seqres->prob = p;
	fprintf(stdout, "seqres->prob est :%f\n", seqres->prob);

	simprf->p = p;
	if ((p >= maxp) || (p > 1))
		p = 1;
	facteur = (1.0 - p);
	fctr = 1;
	simprf->text = (char *)malloc(strlen(line) + 1);
	strcpy(simprf->text, line);
	fprintf(stdout, "simprf->text %s\n", simprf->text);
	/**********************************************************************************/

	while (endofdbseq == 0)
	{

		/**************************************************************************/
		/**************************************************************************/
		/**** Huuhhhh.... t'looks like we've run off the file and should stop *****/

		if (!fgets(line, 256, file) || (feof(file) != 0))
		{
			line[0] = '\0';
			endofdbseq = 1;
		}
		/**************************************************************************/

		/**************************************************************************/
		/**** This is supposed to be the end of the BLAST results, we don't    ****/
		/**** need to read the file any further and may leave                  ****/

		if ((strncmp(line, "WARNING:", 8) == 0) || (strncmp(line, "  Database:", 11) == 0) || (strncmp(line, "Parameters:", 11) == 0))
		{
			line[0] = '\0';
			endofdbseq = 1;
			simprf->next = NULL;
		}
		/**************************************************************************/

		/**************************************************************************/
		/**** The current line starts with "Query", we are therefore reading  *****/
		/**** the alignment into *seq                                         *****/

		if (strncmp(line, "Query", 5) == 0)
		{
			sscanf((char *)(strpbrk(line, "0123456789")), "%d", &debut);
			sscanf((char *)(strrchr(line, ' ')), "%d", &end);
			dline = strlen(line) - strlen(strpbrk(line + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ-"));

			ptrstr = (char *)(strrchr(line, ' '));
			*ptrstr = '\0';
			strcpy(line, (char *)(line + dline));

			if (ok == 0)
			{
				queryseq = (char *)malloc(strlen(line) + 1);
				*queryseq = '\0';
			}
			else
			{
				queryseq = (char *)malloc(strlen(line) + strlen(queryseq) + 1);
				//fprintf(stdout, "la query seq est %s\n\n", queryseq);
			}
			strcat(queryseq, line);
			//fprintf(stdout,"querysec %s\n",queryseq);

			fgets(line, 256, file);

			line[strlen(line) - 1] = '\0';
			n = (end - debut + 1) - strlen((char *)(line + dline));
			if (n > 0)
				strncat(line, "                                                            ", n);

			if (ok == 0)
			{
				begin = debut;
				seq = (char *)realloc(seq, strlen(line) + 1);
				*seq = '\0';
			}
			else
			{
				seq = (char *)realloc(seq, strlen(line) + strlen(seq) + 1);
				//fprintf(stdout,"seq1 est : %s\n", seq);
			}

			strcat(seq, (char *)(line + dline));
			//fprintf(stdout, "seq : %s\n", seq);

			ok = 1;
		}
		/**************************************************************************/

		/**************************************************************************/
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
				//fprintf(stdout, "seqhsp : %s\n", seqhsp);
				okhsp = 1;
			}
		}
		/**************************************************************************/

		/**************************************************************************/
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
			//fprintf(stdout, "simprf->text %s \n", simprf->text);
			sscanf((char *)(strchr(line, '(') + 1), "%d", &taux);
			sscanf((char *)(strchr(line, '=') + 1), "%d", &naas);
			simprf->pcid = taux;
			fprintf(stdout, "La simprftext est : %s\n", simprf->text);


			//printf("le taux est de %u\n", simprf->pcid);
			simprf->nid = naas;
			//printf("naas est de %u\n", simprf->nid);
		}
		/**************************************************************************/

		/**************************************************************************/
		/**** At this point, we have finished reading the current HSP and are  ****/
		/**** ready to deal with it                                            ****/

		if (((strchr("[1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz>", *line)) && ((strncmp(line, "Query", 5) != 0) && (strncmp(line, "Sbjct", 5) != 0) && (strncmp(line, "Score", 5) != 0) && (strncmp(line, "Identities", 10) != 0))) || (strncmp(line, "WARNING:", 8) == 0) || (strncmp(line, "  Database:", 11) == 0) || (strncmp(line, "Score", 5) == 0) || (strncmp(line, " Score", 6) == 0) || (strncmp(line, "Parameters:", 11) == 0))
		{
			/**************************************************************************/
			/**** Let's check that the current database sequence doesn't contain   ****/
			/**** too many low complexity subsequences in the region where the HSP ****/
			/**** has been found. Otherwise, we simply reject this HSP (p set to 1)****/

			if (filter(seqhsp, seq) == 0)
			{
				p = 1;
			}
			/**************************************************************************/

			simprf->hsp = seqhsp;
			simprf->queryseq = queryseq;
			simprf->aln = seq;
			simprf->next = NULL;
			queryseq = NULL;
			seq = NULL;

			/**************************************************************************/

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

			if (gapped)
				simprf = handlegaps(simprf);

			/**************************************************************************/
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
			/**************************************************************************/
			if (*line == '>')
				endofdbseq = 1;
		}
	}
	fprintf(stdout, "seqres->desc %s\n\n", seqres->desc);


#ifdef DEBUG
	printf("%s\n", seqres->desc);
#endif

	for (simprf = seqres->sim; simprf != NULL; simprf = simprf->next)
	{
		seqhsp = simprf->hsp;
		seq = simprf->aln;
		begin = simprf->begin + 1;
		end = simprf->end + 1;
		begdb = simprf->begdb + 1;
		enddb = simprf->enddb + 1;

		simprf->prf = (double *)malloc(sizeof(double) * (end - begin + 1));
		p = simprf->p;
		if ((p >= maxp) || (p > 1))
			p = 1;
		facteur = (1 - p);
		fctr = 1;

#ifdef DEBUG
		printf("Query %6d %s %d\n", begin, simprf->queryseq, end);
		printf("             %s\n", seq);
		printf("Sbjct %6d %s %d\n\n\n", begdb, seqhsp, enddb);
		fflush(NULL);
#endif

		/**************************************************************************/
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

			/****************************************************************/
			/*** Aligned Aas in Query sequence and Database sequence are ****/
			/*** identical                                               ****/

			if ((*(seq + i - begin + 1) != '+') && (*(seq + i - begin + 1) != ' ') && (*(seq + i - begin + 1) != 'x'))
			{
				*ptr = facteur * identique;
				*simptr = ID * fctr;
				//printf("le facteur de similitude est : %u\n", fctr);

				ptrstr = (char *)(conserved + i);
				*ptrstr = *(seq + i - begin + 1);

				/****************************************************************/
			}
			else
			{
				/********************************************************/
				/*** Aligned Aas in Query and Database sequences are  ***/
				/*** similar                                          ***/

				if (*(seq + i - begin + 1) == '+')
				{
					*ptr = identique * facteur / 2;
					*simptr = SIM * fctr;

					/********************************************************/
				}
				else
				{
					/************************************************/
					/*** Well... actually they're different       ***/

					*ptr = identique * NETRA * facteur;
					*simptr = RIEN * fctr;
					/************************************************/
				}
			}
		}
		/**************************************************************************/
	}
	//fprintf(stdout, "le smptr est de : %lf\n", *simptr);
	/**************************************************************************/

	/**********************************************************************************/

	/**********************************************************************************/
	/**** Bye, bye...  ****************************************************************/

	/*
	simprf->queryseq = queryseq;
	simprf->aln = seq;
	simprf->hsp = seqhsp;
	simprf->next = NULL;
	*/
	//printf("Le profil est : %lf\n", *profil);
	return profil;
}
/**********************************************************************************/
