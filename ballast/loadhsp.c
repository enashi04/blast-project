#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "macros.h"
#include "readargs.h"
#include "types.h"

extern int filter(char *seqhsp, char *seq);
extern SimPrf *handlegaps(SimPrf *simprf);

double ValueMaxP(double maxp, char type);
void initSimprf(SeqHSP *seqres, SimPrf *simprf, double maxp, char type);
void readDescOfDB(char *line);
void verifLine(char *line);
void seqAccess(char *ptrstr, SeqHSP *seqres);
void scoreLine(char *outtext, char *line, char *begline, FILE *file, SeqHSP *seqres);
void endOfBlast(char *line, SimPrf *simprf, int endofdbseq);
void queryCurrentAlignment(FILE *file, char *line, int dline, char *ptrstr, char *queryseq, int end, int debut, int begin, char *seq, int ok);
void subjectCurrentAlignment(char *line, int dline, int dline1, int dline2, char *startline, int endofdbseq, SimPrf *simprf, char *ptrstr, int okhsp, int begdb, int debdb, int enddb, char *seqhsp);
void dontKeepHsp(char *line, SimPrf *simprf, int gapped, int taux, int naas);
void dealWithHsp(SeqHSP *seqres,char *line, char *seqhsp, char *seq, SimPrf *simprf, double p, double maxp, int okhsp, char *begline, char *ptrstr, int endofdbseq, char *queryseq, int begin, int end, int begdb, int enddb, int ok, int gapped);

#define NETRA 0

double *loadHSP(SeqHSP *seqres, FILE *file, char *line, int length, char *conserved, double *maxprofile, char type)
{
	double *profil, *ptr, *simptr;
	SimPrf *simprf = NULL;

	int i, j, debut = 0, begin = 0, end = 0;
	int debdb = 0, begdb = 0, enddb = 0;
	int dline = 0;
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
	char *outtext = NULL;
	double p;

	char *ptrstr;
	int identique = 1, taux = 0, naas = 0;

	char *begline;

	double maxp = 0;

	int dline1 = 0, dline2 = 0;
	char *startline = NULL;

	printf("on initialise\n");

	/**** Initialisation **************************************************************/
	// récupérer ou initialiser la valeur de maxp
	maxp = ValueMaxP(maxp, type);
	// initSimprf
	initSimprf(seqres, simprf, maxp, type);

	profil = (double *)malloc(sizeof(double) * length);
	for (i = 0; i < length; i++)
	{
		ptr = (double *)(profil + i);
		*ptr = 0;
	}

	printf("On a fini les initialisations\n");

	/**** Read description of database sequence ***************************************/
	readDescOfDB(line);

	outtext = (char *)realloc(outtext, strlen(line) + 1);
	strcpy(outtext, line);

	verifLine(line);

	seqres->desc = (char *)malloc(strlen(line) + 1);
	strcpy(seqres->desc, line);

	ptrstr = strtok(line, " ");
	seqres->name = (char *)malloc(strlen(ptrstr) + 1);
	strcpy(seqres->name, ptrstr);
	ptrstr = strchr(seqres->name, ':');

	if (ptrstr != NULL)
		*ptrstr = '|';

	ptrstr = strtok(NULL, " ");
	seqAccess(ptrstr, seqres);

	fgets(line, 256, file);

	begline = line;
	if (*begline == ' ')
		begline++;

	/* begline = (char *) (line + strcspn (line," ") + 1);*/
	scoreLine(outtext, line, begline, file, seqres);
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
		endOfBlast(line, simprf, endofdbseq);

		/**** The current line starts with "Query", we are therefore reading  *****/
		/**** the alignment into *seq                                         *****/
		queryCurrentAlignment(file, line, dline, ptrstr, queryseq, end, debut, begin, seq, ok);
		/**************************************************************************/
		/**** The current line starts with "Sbjct", we are therefore reading  *****/
		/**** the database sequence into seqhsp*/
		subjectCurrentAlignment(line, dline, dline1, dline2, startline, endofdbseq, simprf, ptrstr, okhsp, begdb, debdb, enddb, seqhsp);
		/**************************************************************************/
		/**** Don't keep HSPs longer than a tenth of the query sequence length ****/
		/**** AND presenting a 100% identity                                   ****/
		dontKeepHsp(line, simprf, gapped, taux, naas);
		/**************************************************************************/
		/**** At this point, we have finished reading the current HSP and are  ****/
		/**** ready to deal with it                                            ****/
		dealWithHsp(seqres,line,seqhsp,seq,simprf, p, maxp, okhsp, begline, ptrstr, endofdbseq, queryseq,begin, end, begdb, enddb, ok, gapped);
	}

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
			if ((taux == 100) && (naas > (length / 10)))
				identique = 0;
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
	}
	/**** Bye, bye...  ****************************************************************/

	/*
	simprf->queryseq = queryseq;
	simprf->aln = seq;
	simprf->hsp = seqhsp;
	simprf->next = NULL;
	*/
	return profil;
}
/****************************************************************************************************************/
double ValueMaxP(double maxp, char type)
{
	if (getargdouble("-maxp", &maxp) == NULL)
	{
		maxp = DEFMAXP;
	}
	if (type == 'n')
	{
		if (getargdouble("-nmaxp", &maxp) == NULL)
		{
			maxp = DEFMAXP;
		}
	}
	return maxp;
}

void initSimprf(SeqHSP *seqres, SimPrf *simprf, double maxp, char type)
{
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
	seqres->p = 1;
	seqres->prob = maxp;
}

void readDescOfDB(char *line)
{
	if (strncmp(line, "[to_Entrez] [to_Related][to_SRS] ", 33) == 0)
	{
		strcpy(line, &line[33]);
		fprintf(stdout, strcpy(line, &line[33]));
	}
}

void verifLine(char *line)
{
	int i;
	if (line[0] == '>')
		strcpy(line, &line[1]);

	if (strlen(line) <= 61)
	{
		for (i = strlen(line) - 1; i < 61; i++)
			line[i] = ' ';
	}
	line[61] = '\0';
}

void seqAccess(char *ptrstr, SeqHSP *seqres)
{
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
}

void scoreLine(char *outtext, char *line, char *begline, FILE *file, SeqHSP *seqres)
{
	while (strncmp(begline, "Score", 5) != 0)
	{
		printf("Ici on realloque ENCORE la mémoire au outtext\n");
		outtext = (char *)realloc(outtext, strlen(outtext) + strlen(line) + 1);
		printf("BANGER\n");
		strcat(outtext, line);
		fgets(line, 256, file);
		begline = line;
		if (*begline == ' ')
			begline++;
	}
	seqres->outtext = outtext;
}

void endOfBlast(char *line, SimPrf *simprf, int endofdbseq)
{
	if ((strncmp(line, "WARNING:", 8) == 0) || (strncmp(line, "  Database:", 11) == 0) || (strncmp(line, "Parameters:", 11) == 0))
	{

		line[0] = '\0';
		endofdbseq = 1;
		simprf->next = NULL;
	}
}

void queryCurrentAlignment(FILE *file, char *line, int dline, char *ptrstr, char *queryseq, int end, int debut, int begin, char *seq, int ok)
{
	if (strncmp(line, "Query", 5) == 0)
	{
		int n;
		printf("On est dans un if chelou\n");

		sscanf((char *)(strpbrk(line, "0123456789")), "%d", &debut);
		sscanf((char *)(strrchr(line, ' ')), "%d", &end);
		dline = strlen(line) - strlen(strpbrk(line + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ-"));

		ptrstr = (char *)(strrchr(line, ' '));
		*ptrstr = '\0';
		strcpy(line, (char *)(line + dline));

		if (ok == 0)
		{
			printf("On passe par un if ok ==0 \n");
			queryseq = (char *)realloc(queryseq, strlen(line) + 1);
			*queryseq = '\0';
		}
		else
		{
			printf("On va dans lautre if où ok !=0\n");
			queryseq = (char *)realloc(queryseq, strlen(line) + strlen(queryseq) + 1);
		}
		strcat(queryseq, line);

		fgets(line, 256, file);

		line[strlen(line) - 1] = '\0';
		n = (end - debut + 1) - strlen((char *)(line + dline));
		if (n > 0)
			strncat(line, "                                                            ", n);

		if (ok == 0)
		{
			begin = debut;
			printf("On rentre dans un autre ok==0 mais pour seq\n");
			seq = (char *)realloc(seq, strlen(line) + 1);
			*seq = '\0';
		}
		else
		{
			printf("On rentre dans un autre else ok pour seq\n");
			seq = (char *)realloc(seq, strlen(line) + strlen(seq) + 1);
		}

		strcat(seq, (char *)(line + dline));

		ok = 1;
	}
}

void subjectCurrentAlignment(char *line, int dline, int dline1, int dline2, char *startline, int endofdbseq, SimPrf *simprf, char *ptrstr, int okhsp, int begdb, int debdb, int enddb, char *seqhsp)
{
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
				printf("Ici c'est pour seqhsp mais if\n");
				seqhsp = (char *)malloc(strlen(line) + 1);
				*seqhsp = '\0';
				simprf->sens = '+';
				if (debdb > enddb)
					simprf->sens = '-';
			}
			else
			{
				printf("Ici c'est pour seqhsp mais else\n");
				printf("strlen(line) +strlen(seqhsp) +1 = %ld - %ld\n", strlen(line), strlen(seqhsp) + 1);
				seqhsp = (char *)malloc(strlen(line) + strlen(seqhsp) + 1);
				printf("On sort de seqhsp\n");
			}

			strcat(seqhsp, (char *)(line + dline));
			okhsp = 1;
		}
	}
}
void dontKeepHsp(char *line, SimPrf *simprf, int gapped, int taux, int naas)
{
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
}
void dealWithHsp(SeqHSP *seqres,char *line, char *seqhsp, char *seq, SimPrf *simprf, double p, double maxp, int okhsp, char *begline, char *ptrstr, int endofdbseq, char *queryseq, int begin, int end, int begdb, int enddb, int ok, int gapped)
{
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

		if (gapped)
			simprf = handlegaps(simprf);

		/**************************************************************************/
		/**** The current line starts with " Score", we are therefore starting ****/
		/**** to read a new HSP                                                ****/

		begline = line;
		if (*begline == ' ')
			begline++;

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