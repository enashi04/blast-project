#include "XMLtoBLASTP.h"

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

    /*****************************************************************************/
    // verif du nombre de hit si y'en a deux alors on prend en compte le score en *2 et le reste des params en *2.
    // on rentre dans les hit
    // récup du score, expect, Method
    // identitie, positives (en % et en normal)

    // Viens la query midline subject où il faut qu'il y est 64 caracteres

    // à la fin on prends les blastoup_param
    //-> param : matrix / expect/gap_pen/ gap_extends/filter / on s'arretera là peut etre
}

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
void blast_version(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_version";

    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "%s\n\n\n", xmlNodeGetContent(node));
    }
}

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

void blast_db(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_db";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "DataBase: %s\n\n\n\n", xmlNodeGetContent(node));
    }
}

void query_Def(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_query-def";
    int j = 72;
    if (strcmp(name, (const char *)node->name) == 0)
    {
        // affichage
        char content[512] = "Query: ";
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

void query_Length(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_query-len";

    if (strcmp(name, (const char *)node->name) == 0)
    {
        // affichage
        fprintf(output, "Length : %s\n\n", xmlNodeGetContent(node));
        // ici on va mettre le sequences producting significant alignments parce que ballast doit le lire !
        fprintf(output, "                                                                 Score    E\n");
        fprintf(output, "Sequences producing significant alignments:                      (bits) Value\n\n");
        // ici on met la query length pour le calcul de la query cover
    }
}

void iterationNode(xmlNode *node, FILE *output) // fonction récursive ici a faire
{
    char *iteration = "Iteration";
    if (strcmp(iteration, (const char *)node->name) == 0)
    {
        xmlNode *childNode, *child;
        childNode = node->children;
        for (child = childNode; child; child = child->next)
        {
            if (strcmp("Iteration_hits", (const char *)child->name) == 0)
            {
                // deuxième parcours pour enfin rentrer dans hit
                xmlNode *hitChild;
                hitChild = child->children;
                for (childNode = hitChild; childNode; childNode = childNode->next)
                {
                    hitNode(childNode, output);
                }
            }
        }
    }
}

void hitNode(xmlNode *node, FILE *output)
{
    if (strcmp("Hit", (const char *)node->name) == 0)
    {
        //boucle puis on récupère la subject et la longueur
        xmlNode *childNode, *child;
        childNode = node->children;
        for (child = childNode; child; child = child->next)
        {
            if (strcmp("Hit_def", (const char *)child->name) == 0)
            {                     
                char *content = (char *)xmlNodeGetContent(child);
                strcpy(content, replaceWord(content, ">", "& "));
                char newcontent[2048];
                memset(newcontent,0,sizeof(newcontent));
                snprintf(newcontent,sizeof(newcontent),">%s", content);
                //printf("content is %s\n", newcontent);

                int j = 98;
                int len = strlen(newcontent);
                // formatage
                for (int i = 0; i < len; i++)
                {
                    fprintf(output, "%c", newcontent[i]);
                    if ((i + 1) % j == 0)
                    {
                        if (newcontent[i] == ' ' || newcontent[i+1]==' ')
                        {
                            fprintf(output, "\n\t");
                        }
                        else
                        {
                            j++;
                        }
                    }
                }
                fprintf(output, "\n");
            }
            else if(strcmp("Hit_len", (const char *)child->name) == 0){
                char *len= (char *)xmlNodeGetContent(child);
                fprintf(output, "\tLength=%s\n\n", len);
            }
            else if(strcmp("Hit_hsps",(const char *)child->name)==0){
                //on entre dans la partie la plus importante du sujet !!!
                xmlNode* hsp,*hspchild;
                hsp=child->children;
                for(hspchild=hsp; hspchild; hspchild=hspchild->next){
                    if(strcmp("Hsp", (const char *)hspchild->name)==0){
                        hspNode(hspchild, output);
                    }
                }
                
            }
        }
    }
}

void hspNode(xmlNode *node, FILE *output) 
{
    xmlNode *childNode, *child;
    childNode = node->children;
    //valeur à récupérer
    char query_from[8], query_to[8], target_from[8], target_to[8], align_len[8], score[8], evalue[8], hsp_num[8], identity[8], positive[8], gaps[8];
    
    for (child = childNode; child; child = child->next)
    {
        if(strcmp("Hsp_num", (const char*)child->name)==0){
            strcpy(hsp_num,(const char *)xmlNodeGetContent(child));
            
        }
        else if(strcmp("Hsp_score", (const char *)child->name)==0){
            strcpy(score,(const char *)xmlNodeGetContent(child));
            
        }
        else if(strcmp("Hsp_evalue", (const char *)child->name)==0){
            strcpy(evalue,(const char *)xmlNodeGetContent(child));
           
        }
        else if(strcmp("Hsp_query-from", (const char *)child->name)==0){
             strcpy(query_from,(const char *)xmlNodeGetContent(child));
              //
        }
        else if(strcmp("Hsp_query-to", (const char *)child->name)==0){
             strcpy(query_to,(const char *)xmlNodeGetContent(child));
        }
        else if(strcmp("Hsp_hit-from", (const char *)child->name)==0){
             strcpy(target_from,(const char *)xmlNodeGetContent(child));
             //printf("hsp is : %s\n",target_from);
        }
        else if(strcmp("Hsp_hit-to", (const char *)child->name)==0){
             strcpy(target_to,(const char *)xmlNodeGetContent(child));
        }
        else if(strcmp("Hsp_identity", (const char *)child->name)==0){
            strcpy(identity,(const char *)xmlNodeGetContent(child));
        }
        else if(strcmp("Hsp_positive", (const char *)child->name)==0){
             strcpy(positive,(const char *)xmlNodeGetContent(child));
        }
        else if(strcmp("Hsp_gaps", (const char *)child->name)==0){
            strcpy(gaps,(const char *)xmlNodeGetContent(child));
        }
        else if(strcmp("Hsp_align-len", (const char *)child->name)==0){
             strcpy(align_len,(const char *)xmlNodeGetContent(child));
        }
    }
    //Écriture dans le fichier
    int identity_percent = atoi(identity)*100/atoi(align_len);
    int positive_percent = atoi(positive)*100/atoi(align_len);
    int gaps_percent = atoi(gaps)*100/atoi(align_len);
    //printf("identity percent : %u\n", identity_percent);
    fprintf(output, " Score = %s bits, Expect = %s,\n", score, evalue);
    fprintf(output," Identities = %s/%s (%d%%), Positives = %s/%s (%d%%), Gaps = %s/%s (%d%%)\n\n", identity, align_len, identity_percent, positive, align_len, positive_percent, gaps, align_len, gaps_percent);
    //printf("voici les valeurs de %s %s %s %s %s %s %s %s %s %s %s\n", query_from, query_to, target_from, target_to,align_len, score, identity, positive,gaps, hsp_num,evalue);
}