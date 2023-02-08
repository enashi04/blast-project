#include "retrieveInfo.h"
#include "blastInfo.h"
#include "lineage.h"
#include "parameters.h"
#include "fillStructure.h"

char name_hit[MIN_SIZE];
char name_species[32];
char taxoID[MIN_SIZE], parentspecies[MIN_SIZE], ranks[MIN_SIZE], espece[MIN_SIZE], lineage[MAX_SIZE]; // mettre 4096 à lineage
int t_from = 0, t_to = 0;

/**************************************************************************************************************/
/*                            First method : allowing us to enter to the first node                           */
/** Parametre : fichier : XML FILE                                                                            */
/**             mode : Bronze, Silver, Gold                                                                   */
/**             buffer : taxonomy.dat                                                                         */
/**************************************************************************************************************/
void blastOutPut_iteration(xmlDoc *fichier, char *mode, char *buffer)
{
// Declare the three node allowing us to browse the XML FILE
    xmlNode *node, *child, *root;
//GET THE FIRST NODE
    root = xmlDocGetRootElement(fichier);
//CHILD = SUBNOD OF THE ROOT 
    child = root->children;
//WE LOOK FOR EACH NODE TO GET THE LENGTH, THE DEF OF THE QUERY, THE DEFINITION OF BLAST
    char speciesName[MIN_SIZE];
    int query_length = 0;
    for (node = child; node; node = node->next)
    {
        getBlastVersion(node);
        getBlastDB(node);
        getQueryDef(node, speciesName);
        if (strcmp("BlastOutput_query-len", (const char *)node->name) == 0)
        {
            fprintf(output, "\t\t\"query-length\" : \"%s\",\n", xmlNodeGetContent(node));
            query_length = atoi((const char *)xmlNodeGetContent(node));
        }
    }
//WE DISPLAY THE INFOS OF THE SPECIES
    displayQuerySpecies(speciesName, buffer);
    fprintf(output, "\t\t\"hits\": [\n\t\t"); 
// We initialize it to BlastOutput_iteration
    const char *BLASTOUTPUT_NODE_NAME = "BlastOutput_iterations";
//PATH OF SUBNODES
    for (node = child; node; node = node->next)
    {
//CHECK IF WE'RE IN BLASTOUTPUT-ITERATIONS
        if (strcmp(BLASTOUTPUT_NODE_NAME, (const char *)node->name) == 0)
        {
            xmlNode *childnode = node->children;
            /********************************BLASTOUTPUT SUBNODES***************************************/
            for (child = childnode; child; child = child->next)
            {
                /********************************we initialize ITERATION :***********************************/
                /********************it's the beginning of blast analysis***************************************/
                const char *ITERATION = "Iteration";
                if (strcmp(ITERATION, (const char *)child->name) == 0)
                {
                    /********************************ITERATION SUBNODES**************************************/
                    node_Iteration(child, mode, buffer, query_length); // on ajoute la longueur de la query
                }
            }
        }
    }
    free(buffer);
}

/// @brief
/// @param node
/// @param mode
/// @param buffer
void node_Iteration(xmlNode *node, char *mode, char *buffer, int query_length)
{
    xmlNode *child;
    const char *iteration = "Iteration_hits", *hit = "Hit";
    /********************************CHILD = SOUS-NOEUD DU NODE ****************************************/
    child = node->children;
    /***********************************PARCOURS DES SOUS-NOEUD*****************************************/
    for (node = child; node; node = node->next)
    {
        // check if we're on the node "iteration_hits"
        printf("on entre la normalement?");
        if (strcmp(iteration, (const char *)node->name) == 0)
        {
            /********************************CHILDNODE = SUBNODES OF CHILD************************************/
            xmlNode *childNode = node->children;
            // initialisation la structure ici ?
            printf("on entre ici ?");
            SpeciesInfo *speciesInfo = fillStructure(buffer);;

            for (child = childNode; child; child = child->next)
            {
                // check if we're on the node "iteration_hits"
                if (strcmp(hit, (const char *)child->name) == 0)
                {
                    /*************************************ENTRER DANS L'HSP*********************************************/
                    node_HSP(child, mode, getHitAccession(child, mode), getSpecies(child), buffer, query_length, speciesInfo); // ajouter un autre paramètre!
                }
            }
        }
    }
}

/// @brief get the hit accession
/// @param node
/// @param mode
/// @return hit_id, the hit_accession of the target
char *getHitAccession(xmlNode *node, char *mode)
{
    char *hit_id = NULL;
    xmlNode *child;
    const char *name = "Hit_accession";
    child = node->children;
    for (node = child; node; node = node->next)
    {
        if (strcmp(name, (const char *)node->name) == 0)
        {
            hit_id = (char *)xmlNodeGetContent(node);
        }
    }
    return hit_id;
}

/// @brief get the name of the species in the definition of the target (ici ca sera encore à modifier)
/// @param node
/// @return
char *getSpecies(xmlNode *node)
{
    char *content;
    const char *name = "Hit_def";
    xmlNode *child = node->children;
    for (node = child; node; node = node->next)
    {
        if (strcmp(name, (const char *)node->name) == 0)
        {
            content = (char *)xmlNodeGetContent(node);
        }
    }
    int debut = 0, fin = 0;
    for (int i = 0; i < strlen(content); i++)
    {
        if (content[i] == '[')
        {
            debut = i;
        }
        if (content[i] == ']')
        {
            fin = i;
            break;
        }
    }
    int j = 0;
    for (int i = debut + 1; i < fin; i++)
    {
        name_species[j] = content[i];
        j++;
    }
    name_species[j] = '\0';
    return name_species;
}

void node_HSP(xmlNode *node, char *mode, char *hit_id, char *species, char *buffer, int query_length, SpeciesInfo *speciesInfo) // ajout du file taxo
{
    xmlNode *child;
    const char *name, *name2;
    /********************************CHILD = SOUS-NOEUD DU NODE ****************************************/
    child = node->children;
    name = "Hit_hsps";
    name2 = "Hsp";

    for (node = child; node; node = node->next)
    {
        if (strcmp(name, (const char *)node->name) == 0)
        {
            /*****************************CHILDNODE = SOUS-NOEUD DE NODE***************************************/
            xmlNode *childNode;
            childNode = node->children;
            for (child = childNode; child; child=child->next)
            {
                /***************************NOEUD DANS LEQUEL ON SE TROUVE = HSP ?**********************************/
                if (strcmp(name2, (char *)child->name) == 0)
                {
                    if (strcmp(name_hit, "") == 0)
                    {
                        fprintf(output, "\t{\n\t\t\t\t\"hit_accession\" : \"%s\",\n", hit_id);
                        strcpy(name_hit, hit_id);
                    }
                    else
                    {
                        fprintf(output, "\t\t,{\n\t\t\t\t\"hit_accession\" : \"%s\",\n", hit_id);
                    }
                    fprintf(output, "\t\t\t\t\"species\": [\n\t\t\t\t {\n");
                    //ici on va commencer le parcours et le remplissage de la structure
                   //int find =0;
                    printf("on arrive ic \n");
                   

                    while (speciesInfo != NULL)
                    {
                        if(strcmp(speciesInfo->name, species)==0){
                            //affichage des info
                            fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n", speciesInfo->name);
                            fprintf(output, "\t\t\t\t\t\"taxid\":\"%s\",\n", speciesInfo->id);
                            fprintf(output, "\t\t\t\t\t\"parent\":\"%s\",\n", speciesInfo->parent);
                            fprintf(output, "\t\t\t\t\t\"rank\":\"%s\",\n", speciesInfo->rank);
                            // fprintf(output, "\t\t\t\t\t\"lineage\":[\n%s", speciesInfo->lineage);
                            // fprintf(output,"\t\t\t\t\t]\n\t\t\t\t},\n");
                            fprintf(output, "\t\t\t\t},\n");
                            //find =1; 
                            break;
                        }
                        speciesInfo = speciesInfo->next;
                    }
                    // if(find ==0){
                    //     //remplissage + parcours
                    //     while (speciesInfo != NULL)
                    //     {
                    //         if(strcmp(speciesInfo->name, species)==0){
                    //             //affichage des info
                    //             fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n", speciesInfo->name);
                    //             fprintf(output, "\t\t\t\t\t\"taxid\":\"%s\",\n", speciesInfo->id);
                    //             fprintf(output, "\t\t\t\t\t\"parent\":\"%s\",\n", speciesInfo->parent);
                    //             fprintf(output, "\t\t\t\t\t\"rank\":\"%s\",\n", speciesInfo->rank);
                    //             // fprintf(output, "\t\t\t\t\t\"lineage\":[\n%s", speciesInfo->lineage);
                    //             // fprintf(output,"\t\t\t\t\t]\nt\t\t\t},\n");
                    //             fprintf(output, "\t\t\t\t},\n");

                    //            // find =1;

                    //             break;
                    //         }
                    //         speciesInfo = speciesInfo->next;
                    //     }

                    // }

                    /*****************************LASTCHILD = SOUS-NOEUD DE CHILD***************************************/
                    xmlNode *lastchild = child->children;
                    for (childNode = lastchild; childNode; childNode = childNode->next)
                    {
                        /*****************************************MODE BRONZE***********************************************/

                        getHSP(childNode, "Hsp_num", "number of hit", 0);
                        getHSP(childNode, "Hsp_identity", "identity", 0);
                        getHSP(childNode, "Hsp_align-len", "align_len", query_length);
                        getHSP(childNode, "Hsp_gaps", "gaps", 0);
                        getHSP(childNode, "Hsp_query-from", "query_from", 0);
                        getHSP(childNode, "Hsp_query-to", "query_to", 0);
                        getHSP(childNode, "Hsp_hit-from", "target_from", 0);
                        getHSP(childNode, "Hsp_hit-to", "target_to", 0);
                        getHSP(childNode, "Hsp_positive", "positive", 0);
                        getHSP(childNode, "Hsp_evalue", "evalue", 0);
                        getHSP(childNode, "Hsp_score", "score", 0);
                        getHSP(childNode, "Hsp_bit-score", "bitscore", 0);
                    }
                    fprintf(output, "\n");
                }
            }
        }
    }
}

void getHSP(xmlNode *node, const char *name, char *label, int query_length)
{ // ajout d'un autre char *
    if (strcmp(name, (const char *)node->name) == 0)
    {
        /******************************Ajout de la query cover **************************************/

        /********************************récupération du tfrom****************************/
        if (strcmp(name, (const char *)"Hsp_hit-from") == 0)
        {
            t_from = atoi((const char *)xmlNodeGetContent(node));
            fprintf(output, "\t\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
        /********************************récupération du to****************************/
        else if (strcmp(name, (const char *)"Hsp_hit-to") == 0)
        {

            t_to = atoi((const char *)xmlNodeGetContent(node));
            fprintf(output, "\t\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
        else if (strcmp(name, (const char *)"Hsp_align-len") == 0)
        {
           // printf("query_length is %d\n", query_length);
            int querycover = 100 * (t_to - t_from) / query_length;
            fprintf(output, "\t\t\t\t\"query-cover\" : \"%d\",\n", querycover);
            fprintf(output, "\t\t\t\t\"%s\" : \"%s\"\n\t\t}", label, xmlNodeGetContent(node));
        }
        else
        {

            fprintf(output, "\t\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
    }
}
