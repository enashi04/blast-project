/**
 * @file profilBuilding.c
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
#include <ctype.h>
#include <stdbool.h>

#define NETRA 0

extern int filter(char *seqhsp, char *seq);
extern SimPrf *handlegaps(SimPrf *simprf);
void verifLine(char *line);
double getMaxP(double maxp, char type);
char *getOuttext(char *line, char *outtext);
char *getDesc(char *line, SeqHSP *seqres);
char *getName(char *line, char *ptrstr, SeqHSP *seqres);
char *getAccess(char *line, char *ptrstr, SeqHSP *seqres);
char *firstScoreRecovery(char *line, char *outtext, char *begline, FILE *file);
double eValueRecovery(char *line, char *ptrstr, double p);
void fixeValue(double p, double maxp);
char *getText(char *line, SimPrf *simprf);


/**
 * @brief Loads HSPs into seqres structure and derives contribution to profile and similarity profile for rescoring. 
 * @param seqres 
 * @param file 
 * @param line 
 * @param length 
 * @param conserved 
 * @param maxprofile 
 * @param type 
  *@return profil
 */

double *profilBuilding(SeqHSP *seqres, FILE *file, char *line, int length, char *conserved, double *maxprofile, char type){
/*************************************************************************************************************************************************/
/************************************************************Déclaration des variables************************************************************/
/************************************************************************************************************************************************/
    double maxp, p, facteur, fctr;
    double *profil, *ptr, *simptr;
    SimPrf *simprf;
    char *outtext = NULL, *ptrstr, *begline, *queryseq, *seq, *seqhsp, *startline;
    int endofdbseq = 0, ok = 0, okhsp = 0, gapped, taux, naas, begin, end, debut, dline, n;
    int debdb, enddb, begdb, dline2, dline1;
    int identique;
/************************************************************************************************************************************************/
/************************************************************Initialisation de maxp**************************************************************/
/************************************************************************************************************************************************/
    maxp = getMaxP(maxp, type);
/************************************************************************************************************************************************/
/************************************************************Initialisation du profil************************************************************/
/************************************************************************************************************************************************/
    profil = (double *)malloc(sizeof(double) * length);

    for (int i = 0; i < length; i++)
    {
        ptr = (double *)(profil + i); // typecasting double adresse de profil+i
        *ptr = 0;                     // valeur de p de base mais changer en 0
    }
/************************************************************************************************************************************************/
/************************************************************Initialisation de seqres************************************************************/
/*******************************************************************et simprf********************************************************************/
/************************************************************************************************************************************************/    
    seqres->sc = 0;
    seqres->nmatch = 0;
    seqres->type = type;
    seqres->risedup = ' ';
    seqres->sim = (SimPrf *)malloc(sizeof(SimPrf));
    simprf = seqres->sim;
    simprf->text = NULL;
    simprf->score = 2;
    simprf->maxscore = 0;
    simprf->nmatch = 0;
    seqres->p = 1;
    seqres->prob = maxp;
/************************************************************************************************************************************************/
/*****************************************************Récupération de la ligne*******************************************************************/
/******************************************************où on se trouve (1rst line)***************************************************************/
/************************************************************************************************************************************************/
    outtext = getOuttext(line, outtext);
/************************************************************************************************************************************************/
/******************************************************Condition vérifiant la line***************************************************************/
/************************************************************************************************************************************************/    
    verifLine(line);
/************************************************************************************************************************************************/
/******************************************************Récupération de desc**********************************************************************/
/************************************************************************************************************************************************/    
    seqres->desc = getDesc(line, seqres);
/************************************************************************************************************************************************/
/******************************************************Récupération de name**********************************************************************/
/************************************************************************************************************************************************/
    seqres->name = getName(line, ptrstr, seqres);
/************************************************************************************************************************************************/
/******************************************************Récupération de access********************************************************************/
/************************************************************************************************************************************************/
    seqres->access = getAccess(line, ptrstr, seqres);

    fgets(line, 256, file);

    begline = line;
    if (*begline == ' ')
    {
        begline++;
    }
/************************************************************************************************************************************************/
/*******************************************************Mise à jour de l'outtext*****************************************************************/
/*******************************************************(de > jusqu'à la longueur)***************************************************************/
/************************************************************************************************************************************************/
    seqres->outtext = firstScoreRecovery(line, outtext, begline, file);
/*************************************************************************************************************************************************/
/********************************************************On est positionné à la ligne ************************************************************/
/*******************************************************commençant par score et on cherche********************************************************/
/****************************************************************la e-value***********************************************************************/
/*************************************************************************************************************************************************/
    p = eValueRecovery(line, ptrstr, p);

    seqres->prob = p;
    simprf->p = p;
/*************************************************************************************************************************************************/
/*******************************************************Condition fixant la e-value***************************************************************/
/*****************************************************************à 1*****************************************************************************/
/************************************************************************************************************************************************/    
    fixeValue(p, maxp);
/************************************************************************************************************************************************/
/*********************************************************Formule de calculs*********************************************************************/
/************************************************************************************************************************************************/
    facteur = (1.0) * p;
    fctr = 1;
/************************************************************************************************************************************************/
/*************************************************Récupération de la ligne de score**************************************************************/
/************************************************************************************************************************************************/
    simprf->text = getText(line, simprf);
/************************************************************************************************************************************************/
/**************************************************Nous sommes à la ligne commencant*************************************************************/
/*********************************************************par Identities*************************************************************************/
/************************************************************************************************************************************************/
    fgets(line, 256, file);
/************************************************************************************************************************************************/
/**************************************************On parcourt le résultat de l'analyse***********************************************************/
/************************************************************************************************************************************************/
    while (endofdbseq == 0)
    {
/************************************************************************************************************************************************/
/*************************************************************Identities*************************************************************************/
/************************************************************************************************************************************************/
        if (strncmp(line, "Identities", 11) == 0)
        {
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
/************************************************************************************************************************************************/
/**************************************************************Query*****************************************************************************/
/************************************************************************************************************************************************/
        else if (strncmp(line, "Query", 5) == 0)
        {
            // là on recherche d'abord en début de la ligne le numéro de début puis de fin
            sscanf((char *)(strpbrk(line, "0123456789")), "%d", &debut);
            sscanf((char *)(strrchr(line, ' ')), "%d", &end);
            // la dline correspond à la longueur de la séquence (autour de 30 normalement)
            dline = strlen(line) - strlen(strpbrk(line + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ-"));
            ptrstr = (char *)(strrchr(line, ' '));
            *ptrstr = '\0';
            memmove(line, (char *)(line + dline), strlen(line));

            if (ok == 0)
            {
                queryseq = (char *)malloc(strlen(line) + 1);
                *queryseq = '\0';
            }
            else
            {
                queryseq = (char *)realloc(queryseq, strlen(line) + strlen(queryseq) + 1); // it was 1
            }
            strcat(queryseq, strtok(line, " "));
/************************************************************************************************************************************************/
/**************************************************************MidLine***************************************************************************/
/************************************************************************************************************************************************/            
            fgets(line, 256, file);
            line[strlen(line) - 2] = '\0';

            strncpy(line, (line + dline), strlen(line) - dline);

            if (ok == 0)
            {
                begin = debut;
                seq = (char *)malloc(strlen(line) + 1);
                *seq = '\0';
                strcpy(seq, line);
            }
            else
            {
                seq = (char *)realloc(seq, strlen(line) + strlen(seq) + 1); // pb ici
                strcat(seq, line);
            }
            ok = 1;
        }

/************************************************************************************************************************************************/
/**************************************************************Sbjct*****************************************************************************/
/************************************************************************************************************************************************/
        else if (strncmp(line, "Sbjct", 5) == 0)
        {

            sscanf((char *)(strpbrk(line, "0123456789")), "%d", &debdb);
            sscanf((char *)(strrchr(line, ' ')), "%d", &enddb);
            dline1 = strlen(line);
            startline = strpbrk(line + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ-");

            if (startline == NULL)
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
                {
                    *ptrstr = '\0';
                }
                if (okhsp == 0)
                {
                    begdb = debdb;
                    seqhsp = (char *)malloc(strlen(line) + 1);
                    *seqhsp = '\0';
                    simprf->sens = '+';
                    if (debdb > enddb)
                    {
                        simprf->sens = '-';
                    }
                }
                else
                {
                    seqhsp = (char *)realloc(seqhsp, strlen(line) + strlen(seqhsp) + 1);
                }
                strcat(seqhsp, (char *)(line + dline));
                okhsp = 1;
            }
        }
        else if(strncmp(line, " Score", 6)==0){ //on arrive vers le score là
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
				if (p < seqres->prob){
					seqres->prob = p;
                }
				simprf->p = p;
        }
        // si on est à la fin du fichier ou si on arrive à la prochaine analyse alors on met fin
        if (feof(file) != 0 || line[0] == '>' || strncmp(line, "Database", 8) == 0)
        {
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
            {
                simprf = handlegaps(simprf);
            }

            endofdbseq = 1;
        }
        else
        {
            // sinon on va a la ligne suivante
            fgets(line, 256, file);
        }
    }

    /****************************************************SIMPRF à calculer****************************************************/
    // printf("la taille de la séquence est : %ld\n", strlen(simprf->aln));
    // int simprofil[strlen(seq)]; // on crée un tableau qui va contenir les valeurs pour chaque aa (if id, sim, nothing)
    // double averageSimptr;
    // int j=0; //remplir le tableau
	printf("%s\n", seqres->desc);

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
        {
            p = 1;
        }
        facteur = (1 - p);
        fctr = 1;

        /**** Update the profile for this sequence accounting for the current  ****/
        /**** HSP and create the similarity profile for the current HSP        ****/

        for (int i = begin - 1; i < end; i++)
        {
            ptr = (double *)(maxprofile + i);
            if ((taux == 100) && (naas > (length / 10)))
            {
                identique = 0;
            }
            else
            {
                identique = 1;
            }
            *ptr += facteur * identique;

            ptr = (double *)(profil + i);
            // printf("le profil est de %f\n", *profil);
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
            }
            else
            {
                /*** Aligned Aas in Query and Database sequences are  ***/
                /*** similar                                          ***/

                if (*(seq + i - begin + 1) == '+')
                {
                    *ptr = identique * facteur / 2;
                    *simptr = SIM * fctr;
                }
                else
                {
                    /*** Well... actually they're different       ***/
                    *ptr = identique * NETRA * facteur;
                    *simptr = RIEN * fctr;
                }
            }
        }
    }

    fprintf(stdout, "le smptr est de : %lf\n", *simptr);
    return profil;
}

double getMaxP(double maxp, char type)
{
    if (getargdouble("-maxp", &maxp) == NULL)
    {
        maxp = DEFMAXP;
        if (type == 'n')
        {
            if (getargdouble("-nmaxp", &maxp) == NULL)
                maxp = DEFMAXP;
        }
    }
    return maxp;
}

void verifLine(char *line)
{
    if (line[0] == '>')
    {
        memmove(line, &line[1], strlen(line));
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
}

char *getOuttext(char *line, char *outtext)
{
    outtext = (char *)malloc(strlen(line) + 1);
    strcpy(outtext, line); // strcpy
    return outtext;
}

char *getDesc(char *line, SeqHSP *seqres)
{
    seqres->desc = (char *)malloc(strlen(line) + 1);
    strcpy(seqres->desc, line);
    return seqres->desc;
}

char *getName(char *line, char *ptrstr, SeqHSP *seqres)
{
    ptrstr = strtok(line, " ");
    seqres->name = (char *)malloc(strlen(ptrstr) + 1);
    strcpy(seqres->name, ptrstr);

    ptrstr = strchr(seqres->name, ':');
    if (ptrstr != NULL)
    {
        *ptrstr = '|';
    }
    return seqres->name;
}

char *getAccess(char *line, char *ptrstr, SeqHSP *seqres)
{
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
    return seqres->access;
}

char *firstScoreRecovery(char *line, char *outtext, char *begline, FILE *file)
{
    while (strncmp(begline, "Score", 5) != 0)
    {
        outtext = (char *)realloc(outtext, strlen(outtext) + strlen(line) + 1);
        strcat(outtext, line);
        // on va à la ligne suivante
        fgets(line, 256, file);
        begline = line;

        if (*begline == ' ')
        {
            begline++;
        }
    }
    // on met à jour outtext
    return outtext;
}

double eValueRecovery(char *line, char *ptrstr, double p)
{
    ptrstr = (char *)(strstr(line, "e-"));
    if (ptrstr != NULL)
    {
        ptrstr = (char *)(ptrstr + 1);
        *ptrstr = 1;
    }
    // ici on va récupérer la e-value qui va correspondre à p.
    sscanf((char *)(strrchr(line, '=') + 1), "%lf", &p);
    return p;
}

void fixeValue(double p, double maxp)
{
    if ((p >= maxp) || (p > 1))
    {
        p = 1;
    }
}

char *getText(char *line, SimPrf *simprf)
{
    simprf->text = (char *)malloc(strlen(line) + 1);
    strcpy(simprf->text, line);
    printf("line is : %s\n", line);
    return simprf->text;
}