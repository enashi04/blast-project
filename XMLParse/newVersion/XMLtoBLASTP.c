#include "XMLtoBLASTP.h"

/*****************************************TEST*****************************************/
int main(int argc, char **argv)
{
    // ouvrir le fichier en mode lecture
    xmlDoc *xmlfile = xmlReadFile("stdin.xml", NULL, 0);
    FILE *output = fopen("output.blastp", "w");
    xmlNode *child, *root;
    root = xmlDocGetRootElement(xmlfile);
    child = root->children;
    blastInfo(xmlfile, output, child);

    // à la fin on prends les blastoup_param
    //-> param : matrix / expect/gap_pen/ gap_extends/filter / on s'arretera là peut etre
}

/*****************************************REMPLACER LES MOTS DANS UNE CHAINE*****************************************/

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

/*****************************************RÉCUPÉRATION DES INFOS DANS LE XML*****************************************/

void blastInfo(xmlDoc *xmlfile, FILE *output, xmlNode *child)
{
    xmlNode *node;
    for (node = child; node; node = node->next)
    {
/*****************************************VERSION DE BLAST*****************************************/
        blast_version(node, output);
/*****************************************REFERENCE DE BLAST*****************************************/
        blast_reference(node, output);
/*****************************************BDD DE BLAST*****************************************/
        blast_db(node, output);
/*****************************************QUERY*****************************************/
        query_Def(node, output);
/*****************************************LONGUEUR DE LA QUERY*****************************************/
        query_Length(node, output);
/*****************************************NOEUD CONTENANT LES ITERATIONS*****************************************/
        if (strcmp("BlastOutput_iterations", (const char *)node->name) == 0)
        {
            xmlNode *childnode;
            childnode = node->children;
            for (child = childnode; child; child = child->next)
            {
                iterationNode(child, output); 
            }
        }
    }
}
/*****************************************VERSION DE BLAST*****************************************/
void blast_version(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_version";

    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "%s\n\n\n", xmlNodeGetContent(node));
    }
}
/*****************************************REF DE BLAST*****************************************/
void blast_reference(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_reference";

    if (strcmp(name, (const char *)node->name) == 0)
    {
        char content[512] = "Reference: ";
/*****************************************REMPLACEMENT DU CARACTERE HTML EN FR*****************************************/
        strcat(content, (const char *)xmlNodeGetContent(node));
        strcpy(content, replaceWord(content, "&auml;", "ä"));

        int j = 62;
        int len = strlen(content);
/*****************************************FORMATAGE*****************************************/
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
/*****************************************BDD*****************************************/
void blast_db(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_db";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "DataBase: %s\n\n\n\n", xmlNodeGetContent(node));
    }
}
/*****************************************QUERY*****************************************/
void query_Def(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_query-def";
    int j = 72;
    if (strcmp(name, (const char *)node->name) == 0)
    {
<<<<<<< HEAD
        /*****************************************FORMATAGE*****************************************/
        char content[512] = "Query= ";
=======
/*****************************************FORMATAGE*****************************************/
        char content[512] = "Query: ";
>>>>>>> parent of 1ef7e60... Xml to blastp almost done
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
/*****************************************LONGUEUR DE LA QUERY*****************************************/
void query_Length(xmlNode *node, FILE *output)
{
    const char *name;
    name = "BlastOutput_query-len";

    if (strcmp(name, (const char *)node->name) == 0)
    {
        // affichage
        fprintf(output, "Length=%s\n\n", xmlNodeGetContent(node));
        // ici on va mettre le sequences producting significant alignments parce que ballast doit le lire !
        fprintf(output, "                                                                 Score    E\n");
        fprintf(output, "Sequences producing significant alignments:                      (bits) Value\n\n");
        // ici on met la query length pour le calcul de la query cover
    }
}
/*****************************************NOEUD ITERATION*****************************************/
void iterationNode(xmlNode *node, FILE *output)
{
    char *iteration = "Iteration";
    if (strcmp(iteration, (const char *)node->name) == 0)
    {
        xmlNode *childNode, *child;
        childNode = node->children;
        for (child = childNode; child; child = child->next)
        {
/*****************************************NOUS ENTRONS DANS LE NOEUD CONTENANT LES HITS*****************************************/
            if (strcmp("Iteration_hits", (const char *)child->name) == 0)
            {
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
/*****************************************NOUS ENTRONS DANS LE NOEUD CONTENANT LES INFOS DU HIT*****************************************/
void hitNode(xmlNode *node, FILE *output)
{
    if (strcmp("Hit", (const char *)node->name) == 0)
    {
        xmlNode *childNode, *child;
        childNode = node->children;

        for (child = childNode; child; child = child->next)
        {
/*****************************************DEF DU HIT*****************************************/
            if (strcmp("Hit_def", (const char *)child->name) == 0)
            {
                char *content = (char *)xmlNodeGetContent(child);
                strcpy(content, replaceWord(content, ">", "& "));
                char newcontent[2048];
                memset(newcontent, 0, sizeof(newcontent));
/*****************************************MISE EN PLACE DU > POUR QUE BALLAST RECONNAISSE LE HIT*****************************************/
                snprintf(newcontent, sizeof(newcontent), ">%s", content);

                int j = 98;
                int len = strlen(newcontent);
/*****************************************FORMATAGE*****************************************/
                for (int i = 0; i < len; i++)
                {
                    fprintf(output, "%c", newcontent[i]);
                    if ((i + 1) % j == 0)
                    {
                        if (newcontent[i] == ' ' || newcontent[i + 1] == ' ')
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
/*****************************************LONGUEUR DU HIT*****************************************/
            else if (strcmp("Hit_len", (const char *)child->name) == 0)
            {
                char *len = (char *)xmlNodeGetContent(child);
                fprintf(output, "\tLength=%s\n\n", len);
            }
/*****************************************NOEUD CONTENANT D'AUTRES INFOS IMPORTANTES DU HIT*****************************************/
            else if (strcmp("Hit_hsps", (const char *)child->name) == 0)
            {
                // on entre dans la partie la plus importante du sujet !!!
                xmlNode *hsp, *hspchild;
                hsp = child->children;
                for (hspchild = hsp; hspchild; hspchild = hspchild->next)
                {
                    if (strcmp("Hsp", (const char *)hspchild->name) == 0)
                    {
/*****************************************NOEUD HSP*****************************************/
                        hspNode(hspchild, output);
                    }
                }
            }
        }
    }
}
/*****************************************ENTRER DANS LE NOEUD CONTENANT LES INFOS NUMÉRIQUES + SEQUENCES*****************************************/
void hspNode(xmlNode *node, FILE *output)
{
    xmlNode *childNode, *child;
    childNode = node->children;
/*****************************************DECLA DES VALEURS À RÉCUPÉRER*****************************************/
    char align_len[8], score[8], evalue[8], identity[8], positive[8], gaps[8];

    for (child = childNode; child; child = child->next)
    {
/*****************************************SCORE*****************************************/
        if (strcmp("Hsp_score", (const char *)child->name) == 0)
        {
            strcpy(score, (const char *)xmlNodeGetContent(child));
        }
/*****************************************EVALUE*****************************************/
        else if (strcmp("Hsp_evalue", (const char *)child->name) == 0)
        {
            strcpy(evalue, (const char *)xmlNodeGetContent(child));
        }
/*****************************************IDENTITY*****************************************/
        else if (strcmp("Hsp_identity", (const char *)child->name) == 0)
        {
            strcpy(identity, (const char *)xmlNodeGetContent(child));
        }
/*****************************************POSITIVE*****************************************/
        else if (strcmp("Hsp_positive", (const char *)child->name) == 0)
        {
            strcpy(positive, (const char *)xmlNodeGetContent(child));
        }
/*****************************************GAPS*****************************************/
        else if (strcmp("Hsp_gaps", (const char *)child->name) == 0)
        {
            strcpy(gaps, (const char *)xmlNodeGetContent(child));
        }
/*****************************************LONGUEUR DE L'ALIGNEMENT*****************************************/
        else if (strcmp("Hsp_align-len", (const char *)child->name) == 0)
        {
            strcpy(align_len, (const char *)xmlNodeGetContent(child));
        }
    }
/*****************************************PETIT CALCUL POUR RÉCUPÉRER LE % DE CES CHAMPS*****************************************/
    int identity_percent = atoi(identity) * 100 / atoi(align_len);
    int positive_percent = atoi(positive) * 100 / atoi(align_len);
    int gaps_percent = atoi(gaps) * 100 / atoi(align_len);
<<<<<<< HEAD
    if(hsp_num==1){
        fprintf(output, " Score = %s bits, Expect = %s,\n", score, evalue);
        fprintf(output, " Identities = %s/%s (%d%%), Positives = %s/%s (%d%%), Gaps = %s/%s (%d%%)\n\n", identity, align_len, identity_percent, positive, align_len, positive_percent, gaps, align_len, gaps_percent);
        blasting(node, output);
    }
    else{
        fprintf(output, "\n\n Score = %s bits, Expect = %s,\n", score, evalue);
        fprintf(output, " Identities = %s/%s (%d%%), Positives = %s/%s (%d%%), Gaps = %s/%s (%d%%)\n\n", identity, align_len, identity_percent, positive, align_len, positive_percent, gaps, align_len, gaps_percent);
        blasting(node, output);
    }
   
=======
    fprintf(output, " Score = %s bits, Expect = %s,\n", score, evalue);
    fprintf(output, " Identities = %s/%s (%d%%), Positives = %s/%s (%d%%), Gaps = %s/%s (%d%%)\n\n", identity, align_len, identity_percent, positive, align_len, positive_percent, gaps, align_len, gaps_percent);
    blasting(node, output);
>>>>>>> parent of 1ef7e60... Xml to blastp almost done
}
/*****************************************RECUPERATION DES SEQUENCES*****************************************/
void blasting(xmlNode *node, FILE *output)
{
    xmlNode *childNode, *child;
    childNode = node->children;
/*****************************************DEBUT/FIN DE QUERY & SUBJECT*****************************************/
    char query_from[8], query_to[8], target_from[8], target_to[8];
/*****************************************DECLARATION DES SEQUENCES À RÉCUPÉRER*****************************************/
    char queryS[2048], targetS[2048], midlineS[2048];

    for (child = childNode; child; child = child->next)
    {
        if (strcmp("Hsp_query-from", (const char *)child->name) == 0)
        {
            strcpy(query_from, (const char *)xmlNodeGetContent(child));
        }
        else if (strcmp("Hsp_query-to", (const char *)child->name) == 0)
        {
            strcpy(query_to, (const char *)xmlNodeGetContent(child));
        }
        else if (strcmp("Hsp_hit-from", (const char *)child->name) == 0)
        {
            strcpy(target_from, (const char *)xmlNodeGetContent(child));
        }
        else if (strcmp("Hsp_hit-to", (const char *)child->name) == 0)
        {
            strcpy(target_to, (const char *)xmlNodeGetContent(child));
        }
/*****************************************QUERY SEQUENCE*****************************************/
        else if (strcmp("Hsp_qseq", (const char *)child->name) == 0)
        {
            strcpy(queryS, (const char *)xmlNodeGetContent(child));
            printf("la query de base est :%s\n\n",queryS);
        }
/*****************************************SUBJECT SEQUENCE*****************************************/
        else if (strcmp("Hsp_hseq", (const char *)child->name) == 0)
        {
            strcpy(targetS, (const char *)xmlNodeGetContent(child));
            // printf("hsp is : %s\n",target_from);
        }
/*****************************************MIDLINE SEQUENCE*****************************************/
        else if (strcmp("Hsp_midline", (const char *)child->name) == 0)
        {
            strcpy(midlineS, (const char *)xmlNodeGetContent(child));
        }
    }
/*****************************************INTIIALISATION DE NLLES VARIABLES *****************************************/
    int len = strlen(queryS); // qu'on prenne midline ou targetS la taille est la même
    int j = 60, debut =0, fin =60;
    char qseq[128], tseq[128], mseq[128];

/*****************************************FORMATAGE*****************************************/
    for (int i = 0; i < len; i++)
    {
        if ((i + 1) % j == 0)
        {
            char newquery[60], newmidline[60], newtarget[60];
<<<<<<< HEAD
            //compteur
            int debinit = 0;
            int gaps_query=0, gaps_target=0;

            for (int k = debut; k < fin; k++)
            {
                newquery[debinit] = queryS[k];
                newmidline[debinit] = midlineS[k];
                newtarget[debinit] = targetS[k];
=======
            int debinit=0;
            for(int k=debut; k<fin; k++){
                newquery[debinit]=queryS[k];
                newmidline[debinit]= midlineS[k];
                newtarget[debinit]= targetS[k];
>>>>>>> parent of 1ef7e60... Xml to blastp almost done
                debinit++;
                if(queryS[k]=='-'){
                    gaps_query+=1;
                }
                if(targetS[k]=='-'){
                    gaps_target+=1;
                }
            }
<<<<<<< HEAD
            newquery[60] = '\0';
            newmidline[60] = '\0';
            newtarget[60] = '\0';

            snprintf(dq,sizeof(dq), "%d", debutquery);
            //printf("la valeur de dq est : %s\n", dq);
            snprintf(dt,sizeof(dt), "%d", debuttarget);
          
            snprintf(fq,sizeof(fq), " %d", debutquery+59-gaps_query);
            snprintf(ft, sizeof(ft)," %d", debuttarget+59-gaps_target);

            strcpy(qseq, "Query: ");
            strcpy(tseq, "Sbjct: ");
            strcpy(mseq, "            ");
            strcat(qseq, dq);
            strcat(tseq, dt);

            /**********************On rajoute + 60*****************************/

            for (int j = strlen(qseq); j < 12; j++)
            {
                strcat(qseq, " ");
            }
            for (int j = strlen(tseq); j < 12; j++)
            {
                strcat(tseq, " ");
            }

            strcat(qseq, newquery);
            strcat(qseq, fq);
            strcat(mseq, newmidline);
            strcat(tseq, newtarget);
            strcat(tseq, ft);

            fprintf(output,"%s\n%s\n%s\n\n", qseq, mseq,tseq);
            debutquery = atoi(fq)+1;
            debuttarget = atoi(ft)+1;
            //pour la boucle de remplissage de newquery/newtarget/newmidline
            debut = fin;
            fin = fin + 60;
        }
        else if (i + 1 == len)
        {
            //printf("la longueur est :%u\n", len-debut);
            char newquery[60], newmidline[60], newtarget[60];
            int debinit = 0;
            for (int k = debut; k < len; k++)
            {           
                    newquery[debinit] = queryS[k];
                    newmidline[debinit] = midlineS[k];
                    newtarget[debinit] = targetS[k];
=======
            newquery[60]='\0';
            newmidline[60]='\0';
            newtarget[60]='\0';

            strcpy(qseq, "Query: ");
            strcpy(tseq, "Subject: ");
            strcpy(mseq,"             ");
            strcat(qseq, query_from);
            strcat(tseq, target_from);
            
            // strcat(qseq, " ");
            // strcat(tseq," ");
            // strcat(qseq, strcat())

            //ici on insère
           printf("la query is : %s\n", newquery);
            debut = fin; 
            fin = fin+60;
         }
        else if(i+1 ==len){
            char newquery[len], newmidline[len], newtarget[len];
            int debinit=0;
            for(int k=debut; k<len; k++){
                if(queryS[k]!='\0'){
                    newquery[debinit]=queryS[k];
                    newmidline[debinit]= midlineS[k];
                    newtarget[debinit]= targetS[k];
>>>>>>> parent of 1ef7e60... Xml to blastp almost done
                    debinit++;
            }
<<<<<<< HEAD
            newquery[ debinit] = '\0';
            newmidline[debinit] = '\0';
            newtarget[debinit] = '\0';

            snprintf(dq,sizeof(dq), "%d", debutquery);
            //printf("la valeur de dq est : %s\n", dq);
            snprintf(dt,sizeof(dt), "%d", debuttarget);
            snprintf(fq, sizeof(fq), " %d", finquery);
            snprintf(ft, sizeof(ft), " %d", fintarget); 

            strcpy(qseq, "Query: ");
            strcpy(tseq, "Sbjct: ");
            strcpy(mseq, "            ");
            strcat(qseq, dq);
            strcat(tseq, dt);
=======
            newquery[len]='\0';
            newmidline[len]='\0';
            newtarget[len]='\0';
          
        }
//             printf("on entre ici ensuite");
// /**************************sous format Query 25 : LRTPLAAIQGGGXXXXX********************************/
>>>>>>> parent of 1ef7e60... Xml to blastp almost done


<<<<<<< HEAD
            for (int j = strlen(qseq); j < 12; j++)
            {
                strcat(qseq, " ");
            }
            for (int j = strlen(tseq); j < 12; j++)
            {
                strcat(tseq, " ");
            }

            strcat(qseq, newquery);
            strcat(qseq, fq);
            strcat(mseq, newmidline);
            //printf("la newmidline est %s\n", mseq);
            strcat(tseq, newtarget);
            strcat(tseq, ft);

            //mettre un \0 à la fin
            qseq[strlen(qseq)]='\0';
            mseq[strlen(qseq)]='\0';
            tseq[strlen(qseq)]='\0';

            fprintf(output,"%s\n%s\n%s\n\n\n", qseq, mseq,tseq);
        }
=======
//             for (int j = strlen(qseq); j < 14; j++)
//             {
//                 strcat(qseq, " ");
//             }
//             for (int j = strlen(tseq); j < 14; j++)
//             {
//                 strcat(tseq, " ");
//             }
// /*****************************************FORMATAGE*****************************************/
//             strcat(qseq,fillQuery);
//             strcat(mseq,fillMidline);
//             strcat(tseq,fillTarget);

//             // strcat(qseq,cquery);
//             // strcat(mseq, ctarget);
//             // strcat(tseq, cmidline);
//             char *space = " ";
//             strcat(qseq, strcat(space,query_to));
//             strcat(tseq, strcat(space,target_to));
//             // printf("qseq is %s\n", qseq);

//             fprintf(output, "%s\n", qseq);
//             fprintf(output, "%s\n", mseq);
//             fprintf(output, "%s\n\n", tseq);  
//         }
>>>>>>> parent of 1ef7e60... Xml to blastp almost done
    }
}