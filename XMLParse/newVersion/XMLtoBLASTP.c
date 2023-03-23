#include "XMLtoBLASTP.h"

// /*****************************************RÉCUPÉRATION DES INFOS DANS LE XML*****************************************/
// int main(int argc, char **argv)
// {
//     xmlDoc *xmlfile = xmlReadFile("stdin.xml", NULL, 0);
//     xmlNode *root = xmlDocGetRootElement(xmlfile);
//     xmlNode *child = root->children;

//     char *database = (char *)malloc(sizeof(char));
//     char blastInfo[MAXI_SIZE];
//     strcpy(blastInfo,getInfoBlast(child, database));
//     convertToBlastP(xmlfile, child, blastInfo, database);
// }

char *getInfoBlast(xmlNode *node, char *database)
{
    char *content = (char *)malloc(sizeof(char)*MAXI_SIZE);

    for (xmlNode *child = node; child; child = child->next)
    {
        if (strcmp("BlastOutput_version", (char *)child->name) == 0)
        {
            strcpy(content, (char *)xmlNodeGetContent(child));
            strcat(content, "\n\n\n");
        }
        if (strcmp("BlastOutput_reference", (const char *)child->name) == 0)
        {
            blast_reference(child, content);
        }
         if (strcmp("BlastOutput_db", (const char *)child->name) == 0)
        {
            strcat(content, "Database: ");
            strcat(content, (char *)xmlNodeGetContent(child));
            strcpy(database, (char *)xmlNodeGetContent(child));
            strcat(content, "\n\n\n\n");
            break;
        }
    }
    return content;
}

void convertToBlastP(xmlDoc *xmlfile, xmlNode *child, char *blastInfo, char *database)
{
    int i = 1;
    for (xmlNode *node = child; node; node = node->next)
    {
        if (strcmp("BlastOutput_iterations", (const char *)node->name) == 0)
        {
            xmlNode *childnode = node->children;
            for (child = childnode; child; child = child->next)
            {
                if (strcmp("Iteration", (const char *)child->name) == 0)
                {
                    // création du nom de fichier
                    char filename[] = BLAST_FILE;
                    filename[8] = i + '0';
                    filename[9]='\0';
                    //printf("blastinfo is %s\n", blastInfo);

                    strcat(filename, ".blastp");

                    FILE *output = fopen(filename,"w");
                    fprintf(output,"%s", blastInfo);
                    iterationNode(child, output, database);

                    i++;
                }
            }
        }
    }
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

/*****************************************REF DE BLAST*****************************************/
void blast_reference(xmlNode *node, char *content)
{
    char var[512] = "Reference: ";
    /*****************************************REMPLACEMENT DU CARACTERE HTML EN FR*****************************************/
    strcat(var, (const char *)xmlNodeGetContent(node));
    strcpy(var, replaceWord(var, "&auml;", "ä"));

    int j = 62;
    int len = strlen(var);
    char *ncontent = (char *)malloc(sizeof(char) * len);
    /*****************************************FORMATAGE*****************************************/
    for (int i = 0; i < len; i++)
    {
        ncontent[i] = var[i];
        // fprintf(content , "%c", var[i]);
        if ((i + 1) % j == 0)
        {
            if (var[i] == ' ')
            {
                // fprintf(content, "\n");
                ncontent[i] = '\n';
            }
            else
            {
                j++;
            }
        }
    }
    strcat(ncontent, "\n\n\n\n");
    strcat(content, ncontent);

}

/*****************************************QUERY*****************************************/
void query_Def(xmlNode *node, FILE *output)
{
    int j = 72;
    if (strcmp("Iteration_query-def", (const char *)node->name) == 0)
    {

        /*****************************************FORMATAGE*****************************************/
        char content[512] = "Query= ";
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
  
    if (strcmp("Iteration_query-len", (const char *)node->name) == 0)
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
void iterationNode(xmlNode *node, FILE *output, char *database)
{
    char *iteration = "Iteration";
    if (strcmp(iteration, (const char *)node->name) == 0)
    {
        xmlNode *childNode, *child;
        childNode = node->children;
        for (child = childNode; child; child = child->next)
        {
            query_Def(child,output);
            query_Length(child, output);
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
            else if(strcmp("Iteration_stat",(const char *)child->name) == 0){
                xmlNode *hitChild;
                hitChild = child->children;
                for (childNode = hitChild; childNode; childNode = childNode->next)
                {
                    statNode(childNode, output, database);
                }
            }
        }
    }
}

void statNode(xmlNode *node,FILE  *output, char *database){
   
    char kappa[6], lambda[6], entropy[6];
    xmlNode *child,*childNode = node->children; 
    for(child=childNode; child; child=child->next){
        if(strcmp("Statistics_db-num", (const char *)child->name)==0){
            fprintf(output, "  Database: %s\n", database);
            //ajout de la date ici
            time_t now;
            time(&now);
            fprintf(output,"\tPosted date : %s", ctime(&now));
            fprintf(output, "  Number of letters: %s\n ", (char *)xmlNodeGetContent(child));
        }
        else if(strcmp("Statistics_db-len", (const char*)child->name)==0){
            fprintf(output, " Number of sequences: %s\n ", (char *)xmlNodeGetContent(child));
        }       
        else if(strcmp("Statistics_kappa", (const char *)child->name)==0){
            strcpy(kappa, (const char*)xmlNodeGetContent(child));
            //kappa =xmlNodeGetContent(node);
        }
        else if(strcmp("Statistics_lambda", (const char *)child->name)==0){
            strcpy(lambda, (const char*)xmlNodeGetContent(child));
            //lambda = xmlNodeGetContent(node);
        }
        else if(strcmp("Statistics_entropy", (const char *)child->name)==0){
            strcpy(entropy, (const char*)xmlNodeGetContent(child));
            // entropy=xmlNodeGetContent(node);
            fprintf(output,"Lambda     K     H\n   %s    %s    %s", lambda, kappa, entropy);
            break;
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
    int hsp_num;

    for (child = childNode; child; child = child->next)
    {
        /***************************************HSP_NUM*****************************************/
        if(strcmp("Hsp_num",(const char *)child->name)==0){
            hsp_num=atoi((char *)xmlNodeGetContent(child));
            //strcpy(hsp_num,(const char*)xmlNodeGetContent(child));
        }
        /*****************************************SCORE*****************************************/
        else if (strcmp("Hsp_score", (const char *)child->name) == 0)
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
   
}
/*****************************************RECUPERATION DES SEQUENCES*****************************************/
void blasting(xmlNode *node, FILE *output)
{

    xmlNode *childNode, *child;
    childNode = node->children;
    /*****************************************DEBUT/FIN DE QUERY & SUBJECT*****************************************/
    char query_from[8], query_to[8], target_from[8], target_to[8];
    /*****************************************DECLARATION DES SEQUENCES À RÉCUPÉRER*****************************************/
    char queryS[MAXI_SIZE], targetS[MAXI_SIZE], midlineS[MAXI_SIZE];

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
            //printf("la query de base est :%s\n\n",queryS);
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
    //printf("on entre icifa ?\n");

    /*****************************************INTIIALISATION DE NLLES VARIABLES *****************************************/
    int len = strlen(queryS); // qu'on prenne midline ou targetS la taille est la même
    //initialisation des valeurs pour la découpe des séquences 60-60
    int j = 60, debut = 0, fin = 60;
    // on récupère le début des query/target sous format int
    int debutquery = atoi(query_from), debuttarget = atoi(target_from), finquery = atoi(query_to), fintarget=atoi(target_to);
    //qseq tseq mseq : séquences fragmenté de ces séquences
    char qseq[128], tseq[128], mseq[128];

    /*****************************************FORMATAGE*****************************************/
    char dq[5], dt[5], fq[5], ft[8]; //début/ fin query - target en char * pour pouvoir le concaténer

    for (int i = 0; i < len; i++)
    {
        if ((i + 1) % j == 0)
        {
            //initialisation des variables pour le remplissage des 60AA
            char newquery[60], newmidline[60], newtarget[60];
            //compteur
            int debinit = 0;
            int gaps_query=0, gaps_target=0;

            for (int k = debut; k < fin; k++)
            {
                newquery[debinit] = queryS[k];
                newmidline[debinit] = midlineS[k];
                newtarget[debinit] = targetS[k];
                debinit++;
                if(queryS[k]=='-'){
                    gaps_query+=1;
                }
                if(targetS[k]=='-'){
                    gaps_target+=1;
                }
            }
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
           // printf("tseq : %s\n", tseq);
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
                debinit++;
            }
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
            

            qseq[strlen(qseq)+1]='\0';
            mseq[strlen(qseq)+1]='\0';
            tseq[strlen(qseq)+1]='\0';

            fprintf(output,"%s\n%s\n%s\n\n\n", qseq, mseq,tseq);
        }
    }
}