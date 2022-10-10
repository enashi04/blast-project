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

double getMaxP(double maxp, char type);
char *getOuttext(char *line, char *outtext);
char *getDesc(char *line, SeqHSP *seqres);
char *getName(char *line, char *ptrstr, SeqHSP *seqres);
char *getAccess(char *line, char *ptrstr, SeqHSP *seqres);
char *firstScoreRecovery(char *line, char *outtext, char *begline, FILE *file);
double eValueRecovery(char *line, char *ptrstr, double p);
void fixeValue(double p, double maxp);
char *getText(char *line, SimPrf *simprf);

double *profilBuilding(SeqHSP *seqres, FILE *file, char *line, int length, char *conserved, double *maxprofile, char type)
{
    // déclaration des variables
    double maxp, p, facteur, fctr;
    double *profil, *ptr, *simptr;
    SimPrf *simprf;
    char *outtext = NULL, *ptrstr, *begline, *queryseq, *seq, *seqhsp, *startline;
    int endofdbseq = 0, ok = 0, okhsp = 0, gapped, taux, naas, begin, end, debut, dline, n;
    int debdb, enddb, begdb, dline2, dline1;
    int identique;

    // si on a comme argument lors de l'exécution du programme maxp ou nmaxp : pour l'instant pas notre cas

    maxp = getMaxP(maxp, type);

    // initialisation du profil
    profil = (double *)malloc(sizeof(double) * length);

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
    simprf->score = 2;
    simprf->maxscore = 0;
    simprf->nmatch = 0;
    seqres->p = 1;
    seqres->prob = maxp;
    // là on récupère la ligne dans laquelle nous nous trouvons
    outtext = getOuttext(line, outtext);

    // condition qui permet de vérifier si ou non il y'a un > permettant ainsi d'identifier la target/query
    verifLine(line);

    // récupération de desc
    seqres->desc = getDesc(line, seqres);
    // récupération de name
    seqres->name = getName(line, ptrstr, seqres);

    // récupération de access
    seqres->access = getAccess(line, ptrstr, seqres);

    // on va à la ligne suivante
    fgets(line, 256, file);

    begline = line;
    if (*begline == ' ')
    {
        begline++;
    }
    // Mise à jour de l'outtext si on n'est pas dans le score (normalement on y est ...)
    seqres->outtext = firstScoreRecovery(line, outtext, begline, file);

    // recherche la première occurence dans laquelle se trouve =e- pour la e-value
    p = eValueRecovery(line, ptrstr, p);

    seqres->prob = p;
    simprf->p = p;

    // condition permettant de fixer la e-value <=1
    fixeValue(p, maxp);

    facteur = (1.0) * p;
    fctr = 1;

    simprf->text = getText(line, simprf);
    fgets(line, 256, file);

    while (endofdbseq == 0)
    {
        if (strncmp(line, "Identities", 11) == 0)
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
            printf("on passe là\n");
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

            /**********************************************MIDDLE SEQUENCE**********************************************/
            fgets(line, 256, file);

            line[strlen(line) - 2] = '\0';

            /*12 OU 13 REVOIR LES NOUVEAUX FORMATS DE FICHIERS BLASTP*/

            strncpy(line, (line + dline), strlen(line) - dline); // a modifier

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

        // On est à la subject
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

    // là on a tout récupéré !! du coup on passe direct à la suite
    // printf("query %s\n\n\n", queryseq);
    // printf("la séquence hsp est %s\n", seqhsp);

    /****************************************************SIMPRF à calculer****************************************************/
    // printf("la taille de la séquence est : %ld\n", strlen(simprf->aln));
    // int simprofil[strlen(seq)]; // on crée un tableau qui va contenir les valeurs pour chaque aa (if id, sim, nothing)
    // double averageSimptr;
    // int j=0; //remplir le tableau
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
    // fprintf(stdout, "le smptr est de : %lf\n", *simptr);
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
    return simprf->text;
}