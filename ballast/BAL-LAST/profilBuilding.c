/**
 * @file loadhspv2.c
 * @author Ihsane E.
 * @brief
 * @version 0.1
 * @date 2022-09-26
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

#define NETRA 0

double *profilBuilding(SeqHSP *seqres, FILE *file, char *line, int length, char *conserved, double *maxprofile, char type)
{
    // dans quelle ligne on se situe
    double maxp, p, facteur, fctr;
    double *profil, *ptr, *simptr;
    SimPrf *simprf;
    char *outtext = NULL, *ptrstr, *begline, *queryseq, *seq, *seqhsp, *startline;
    int endofdbseq = 0, ok, okhsp, gapped, taux, naas, begin, end, debut, dline, n;
    int debdb, enddb, begdb, dline2, dline1;

    printf("Nous nous trouvons dans la ligne : %s\n", line);

    // si on a comme argument lors de l'exécution du programme maxp ou nmaxp
    // pour l'instant pas notre cas
    if (getargdouble("-maxp", &maxp) == NULL)
        maxp = DEFMAXP;
    if (type == 'n')
    {
        if (getargdouble("-nmaxp", &maxp) == NULL)
            maxp = DEFMAXP;
    }

    // initialisation du profil
    profil = (double *)malloc(sizeof(double) * length); // pb ici

    for (int i = 0; i < length; i++)
    {
        ptr = (double *)(profil + i); // typecasting double adresse de profil+i
        *ptr = 0;                     // valeur de p de base mais changer en 0
    }

    // initialisation de seqres et simprf
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

    // là on récupère la ligne dans laquelle nous nous trouvons
    outtext = (char *)malloc(strlen(line) + 1);
    strcpy(outtext, line);

    // condition qui permet de vérifier si ou non il y'a un > permettant ainsi d'identifier
    // la target/query
    if (line[0] == '>')
    {
        strcpy(line, &line[1]);
    }
    // taille de la target (non de la séquence mais de l'identification)
    if (strlen(line) <= 90)
    {
        for (int i = strlen(line) - 1; i < 90; i++)
        {
            line[i] = ' ';
        }
    }

    line[90] = '\0';

    // récupération de desc
    seqres->desc = (char *)malloc(strlen(line) + 1);
    strcpy(seqres->desc, line);
    // récupération de name
    ptrstr = strtok(line, " ");
    seqres->name = (char *)malloc(strlen(ptrstr) + 1);
    strcpy(seqres->name, ptrstr);

    ptrstr = strchr(seqres->name, ':');
    if (ptrstr != NULL)
    {
        *ptrstr = '|';
    }

    // récuparation de access
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
    printf("nous somme à la ligne suivante : %s\n", line);
    begline = line;
    if (*begline == ' ')
    {
        begline++;
    }
    printf("begline is %s\n", begline);
    // comparaison entre la ligne où on se trouve actuellement et la ligne finale avant
    // l'analyse
    while (strncmp(begline, "Score", 5) != 0)
    {
        outtext = (char *)realloc(outtext, strlen(outtext) + strlen(line) + 1);
        strcpy(outtext, line);
        // on va à la ligne suivante
        fgets(line, 256, file);
        begline = line;

        if (*begline == ' ')
        {
            begline++;
        }
    }
    // on met à jour outtext
    seqres->outtext = outtext;
    printf("Nous sommes actuellement à la ligne : %s\n", line);

    // rechercher la première occurence dans laquelle se trouve =e-
    ptrstr = (char *)(strstr(line, "e-"));
    if (ptrstr != NULL)
    {
        ptrstr = (char *)(ptrstr + 1);
        *ptrstr = 1;
    }
    // ici on va récupérer la e-value qui va correspondre à p.
    sscanf((char *)(strrchr(line, '=') + 1), "%lf", &p);
    printf("p est %lf\n", p);
    seqres->prob = p;
    simprf->p = p;

    if ((p >= maxp) || (p > 1))
    {
        p = 1;
    }

    facteur = (1.0) * p;
    fctr = 1;
    simprf->text = (char *)malloc(strlen(line) + 1);
    strcpy(simprf->text, line);
    while (endofdbseq == 0)
    {
        // On parcourt d'abord le score
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
            {
                seqres->prob = p;
            }
            simprf->p = p;
        } // ensuite l'identité
        else if (strncmp(line, " Identities", 11) == 0)
        {
            // si y'a pas de gaps
            if (strstr(line, "Gaps = 0/"))
            {
                gapped = 0;
            }
            else
            {
                gapped = 1;
            }
            simprf->text = (char *)realloc(simprf->text, strlen(simprf->text) + strlen(line) + 14);
            strcat(simprf->text, "             ");
            strcat(simprf->text, line);
            sscanf((char *)(strchr(line, '(') + 1), "%d", &taux);
            sscanf((char *)(strchr(line, '=') + 1), "%d", &naas);
            simprf->pcid = taux;

            simprf->nid = naas;
        }
        // on arrive à la query
        else if (strncmp(line, "Query", 5) == 0)
        {
            // là on recherche d'abord en début de la ligne le numéro de début puis de fin
            sscanf((char *)(strpbrk(line, "0123456789")), "%d", &debut);
            sscanf((char *)(strrchr(line, ' ')), "%d", &end);
            printf("le début est %d; la fin est %d\n", debut, end);
            // la dline correspond à la longueur de la séquence (autour de 30 normalement)
            dline = strlen(line) - strlen(strpbrk(line + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ-"));
            ptrstr = (char *)(strrchr(line, ' '));
            *ptrstr = '\0';
            strcpy(line, (char *)(line + dline));
            printf("la ligne ici est %s\n", line);

            if (ok == 0)
            {
                queryseq = (char *)malloc(strlen(line) + 1);
                *queryseq = '\0';
            }
            else
            {
                queryseq = (char *)realloc(queryseq,strlen(line) + strlen(queryseq) + 1); // it was 1
            }
            strcpy(queryseq, line);
            printf("queryseq %s\n", queryseq);

            fgets(line, 256, file);

            line[strlen(line) - 1] = '\0';
            n = (end - debut + 1) - strlen((char *)(line + dline));
            if (n > 0)
                strncat(line, "                                                            ", n);

            if (ok == 0)
            {
                begin = debut;
                seq = (char *)malloc(strlen(line) * strlen(line));
                *seq = '\0';
            }
            else
            {
                seq = (char *)realloc(seq, strlen(line) + strlen(seq) + 1);
            }

            strcat(seq, (char *)(line + dline)); // strcat ici de base

            ok = 1;
        }
        //on va vers la target
         else if(strncmp(line, "Sbjct", 5) == 0)
		{
			sscanf((char *)(strpbrk(line, "0123456789")), "%d", &debdb);
			sscanf((char *)(strrchr(line, ' ')), "%d", &enddb);

			dline1 = strlen(line);
			startline = strpbrk(line + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ-");
            printf("startline is : %s\n", startline);
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
       
        fgets(line, 256, file);
    }

    // end of while
    return 1;
}