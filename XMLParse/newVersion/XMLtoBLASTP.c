#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/libxml/parser.h"
#include "lib/libxml/tree.h"

// Method replacing a word by another word
char *replaceWord(const char *s, const char *oldW, const char *newW)
{
    char *result;
    int i, cnt = 0; // compteurs
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    // Counting the number of times old word occur in the string
    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], oldW) == &s[i])
        {
            cnt++;
            i += oldWlen - 1;
        }
    }

    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s)
    {
        // compare the substring with the result
        if (strstr(s, oldW) == s)
        {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
    result[i] = '\0';
    return result;
}
// method to retrieve and put the blast version
void blast_version(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_version";

    if (strcmp(name, (const char *)node->name) == 0)
    {
        // affichage
        fprintf(output, "%s\n\n\n", xmlNodeGetContent(node));
    }
}
// method to get the reference of blast
void blast_reference(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_reference";

    if (strcmp(name, (const char *)node->name) == 0)
    {
        char content[512] = "Reference: ";
        strcat(content, (const char *)xmlNodeGetContent(node));
        strcpy(content, replaceWord(content, "&auml;", "ä"));

        int j = 62;
        int len = strlen(content);
        // formatage
        for (int i = 0; i < len; i++)
        {
            fprintf(output, "%c", content[i]);
            if ((i + 1) % j == 0)
            {
                if (content[i] == ' ')
                {
                    fprintf(output, "\n");
                }
                else
                {
                    j++;
                }
            }
        }
        fprintf(output, "\n\n\n\n");
    }
}
// method to get the db
void blast_db(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_db";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        // affichage
        fprintf(output, "DataBase: %s\n\n\n\n", xmlNodeGetContent(node));
    }
}

/**
 *LE NOM DE LA QUERY
 */
void query_Def(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_query-def";
    int j = 72;
    if (strcmp(name, (const char *)node->name) == 0)
    {
        // affichage
        char content[512] = "Query:";
        ;
        strcat(content, (char *)xmlNodeGetContent(node));
        int len = strlen(content);
        for (int i = 0; i < len; i++)
        {
            fprintf(output, "%c", content[i]);
            if ((i + 1) % j == 0)
            {
                if (content[i] == ' ')
                {
                    fprintf(output, "\n\t");
                }
                else
                {
                    j++;
                }
            }
        }
        fprintf(output, "\n\n");
    }
}

/**
 * LONGUEUR DE LA QUERY
 */
void query_Length(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_query-len";

    if (strcmp(name, (const char *)node->name) == 0)
    {
        // affichage
        fprintf(output, "Length : %s\n\n", xmlNodeGetContent(node));
        //ici on va mettre le sequences producting significant alignments parce que ballast doit le lire ! 
        fprintf(output, "                                                                 Score    E\n");
        fprintf(output, "Sequences producing significant alignments:                      (bits) Value\n\n");
        // ici on met la query length pour le calcul de la query cover
    }
}

void iterationNode(xmlNode *child, FILE *output) //fonction récursive ici a faire
{
    char *iteration = "Iteration";
    if (strcmp(iteration, (const char *)child->name) == 0)
    {
        // printf("on est bien rentré je crois\n");
        xmlNode *childnode = child->children;
        for (childnode = child; childnode; childnode = childnode->next)
        {
            if (strcmp("Hit", (const char *)child->name) == 0)
            {
                printf("on est bien dans la section hit qu'on va itérer x fois\n");
            }
        }
        // on va essayer de récupérer les infos qu'on veut
    }
}
// récupération du blastoutput version
void blastInfo(xmlDoc *xmlfile, FILE *output, xmlNode *child)
{
    xmlNode *node;

    for (node = child; node; node = node->next)
    {
        // récupérer la version, la référence, et la base de données
        blast_version(node, output);
        blast_reference(node, output);
        blast_db(node, output);
        query_Def(node, output);
        query_Length(node, output);

        if (strcmp("BlastOutput_iterations", (const char *)node->name) == 0)
        {
            xmlNode *childnode;
            childnode = node->children;
            for (child = childnode; child; child = child->next)
            {
                iterationNode(child, output); // ici ca doit etre une fonction récursive !
            }
        }
    }
}

int main(int argc, char **argv)
{
    // ouvrir le fichier en mode lecture
    xmlDoc *xmlfile = xmlReadFile("stdin.xml", NULL, 0);
    FILE *output = fopen("output.txt", "w");
    xmlNode *child, *root;
    root = xmlDocGetRootElement(xmlfile);
    child = root->children;
    // mettre la boucle ici
    blastInfo(xmlfile, output, child);

    /*
    <Iteration_iter-num>1</Iteration_iter-num>
    <Iteration_query-ID>Query_808952</Iteration_query-ID>
    <Iteration_query-def>sp|P96368|TRCS_MYCTU Sensor histidine kinase TrcS OS=Mycobacterium tuberculosis (strain ATCC 25618 / H37Rv) OX=83332 GN=trcS PE=1 SV=1</Iteration_query-def>
    <Iteration_query-len>509</Iteration_query-len>
  */

    /*****************************************************************************/
    // verif du nombre de hit si y'en a deux alors on prend en compte le score en *2 et le reste des params en *2.
    // on rentre dans les hit
    // récupération de hit_def
    // récup de la longueur
    // récup du score, expect, Method
    // identitie, positives (en % et en normal)

    // Viens la query midline subject où il faut qu'il y est 64 caracteres

    // à la fin on prends les blastoup_param
    //-> param : matrix / expect/gap_pen/ gap_extends/filter / on s'arretera là peut etre
}