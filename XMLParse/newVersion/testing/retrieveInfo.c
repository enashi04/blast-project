/**************************************************************************************************************/
/*                                        RETRIEVEINFO.C                                                      */
/** MAIN FUNCTION FOR GETTING THE JSON FILE                                                                   */
/** Global variable : name_hit                                                                                */
/**                   tfrom, tto :                                                                            */
/**************************************************************************************************************/

#include "retrieveInfo.h"
#include "blastInfo.h"
#include "lineage.h"
#include "parameters.h"
#include "fillStructure.h"

char name_hit[MIN_SIZE];
int t_from = 0, t_to = 0; //voir où on peut le mettre en local au lieu de glo
char stockSPECIES[MAXI_SIZE];
char name_species[MAX_SIZE];


/**************************************************************************************************************/
/*                       blastOutPut_iteration: allowing us to enter to the first node                        */
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
//BLASTOUTPUT SUBNODES
            for (child = childnode; child; child = child->next)
            {
//we initialize ITERATION :it's the beginning of blast analysis
                const char *ITERATION = "Iteration";
                if (strcmp(ITERATION, (const char *)child->name) == 0)
                {
//ITERATION SUBNODES
                    node_Iteration(child, mode, buffer, query_length); // on ajoute la longueur de la query
                }
            }
        }
    }
    free(buffer);
}

/**************************************************************************************************************/
/*                           Node_Iteration: allowing us to enter to the first node                           */
/** Parametre : fichier : XML FILE                                                                            */
/**             node : Node where we are now                                                                  */
/**             query_length :get the length                                                                  */
/**             buffer : taxonomy.dat                                                                         */
/**************************************************************************************************************/
void node_Iteration(xmlNode *node, char *mode, char *buffer, int query_length)
{
    xmlNode *child;
    const char *iteration = "Iteration_hits", *hit = "Hit";
//CHILD = SOUS-NOEUD DU NODE
    child = node->children;
//PARCOURS DES SOUS-NOEUD
    for (node = child; node; node = node->next)
    {
// check if we're on the node "iteration_hits"
        if (strcmp(iteration, (const char *)node->name) == 0)
        {
//CHILDNODE = SUBNODES OF CHILD
            xmlNode *childNode = node->children;
//Initialisation de la structure
            SpeciesInfo *speciesInfo = fillStructure(buffer);
            for (child = childNode; child; child = child->next)
            {
// check if we're on the node "iteration_hits"
                if (strcmp(hit, (const char *)child->name) == 0)
                {
//ENTRER DANS L'HSP
                    node_HSP(child, mode, getHitAccession(child, mode), getSpecies(child), buffer, query_length, speciesInfo); 
                }
            }
        }
    }
}

/**************************************************************************************************************/
/*                           getHitAccession: like the name of the function ...                               */
/** Parametre : node : XML FILE                                                                               */
/**             mode : Bronze, Silver, Gold                                                                   */
/**************************************************************************************************************/
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

/**************************************************************************************************************/
/*                              getSpecies: get the name of the species                                       */
/*                                  in the definition of the target                                           */
/** Parametre : node : Node where we are now                                                                  */
/**************************************************************************************************************/
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
//we're getting just the name of the species (between [ ])
//optionnal : add for loop if the species name is after 'OS'
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
    char name_species[MIN_SIZE];
    for (int i = debut + 1; i < fin; i++)
    {
        name_species[j] = content[i];
        j++;
    }
    name_species[j] = '\0';
    return strdup(name_species);
}

/**************************************************************************************************************/
/*                              Node_HSP: Filling the JSON file                                               */
/** Parametre : node : Node where we are now                                                                  */
/**           : mode : Bronze, Silver, Gold                                                                   */
/**           : hit_id :                                                                                      */
/**           : species : the name of the species                                                             */
/**           : buffer : taxonomy.dat                                                                         */
/**           : query_length : length of the query                                                            */
/**************************************************************************************************************/
void node_HSP(xmlNode *node, char *mode, char *hit_id, char *species, char *buffer, int query_length, SpeciesInfo *speciesInfo) // ajout du file taxo
{
    xmlNode *child;
    const char *name, *name2;
//CHILD = SOUS-NOEUD DU NODE 
    child = node->children;
    name = "Hit_hsps";
    name2 = "Hsp";

    for (node = child; node; node = node->next)
    {
        if (strcmp(name, (const char *)node->name) == 0)
        {
//CHILDNODE = SOUS-NOEUD DE NODE
            xmlNode *childNode;
            childNode = node->children;
            for (child = childNode; child; child=child->next)
            {
//NOEUD DANS LEQUEL ON SE TROUVE = HSP 
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
                    //ici on va commencer le parcours de la structure 
                    //vérifier d'abord que l'information on l'avait pas déjà avant ? 
                    if(strcmp(species, name_species)==0){
                        fprintf(output, "%s\n", stockSPECIES);
                    }
                    else{
                        for(int i =0; i<FILE_SIZE-1; i++){
                            if(strcmp(speciesInfo[i].name, species)==0){
                                char stockage[MAXI_SIZE] = "";
                                strcat(stockage, "\t\t\t\t\t\"name\":\"");
                                strcat(stockage, speciesInfo[i].name);
                                strcat(stockage,"\",\n\t\t\t\t\t\"taxid\":\"" );
                                strcat(stockage, speciesInfo[i].id);
                                strcat(stockage,"\",\n\t\t\t\t\t\"rank\":\"" );
                                strcat(stockage, speciesInfo[i].rank);
                                strcat(stockage, createLineage(speciesInfo, species));
                                fprintf(output,"%s\n", stockage);
                                strcpy(stockSPECIES, stockage);
                                strcpy(name_species, species);

                            }
                        }
                    }
                   
                    //variable globale ici pour stocker ce qu'il y'a a stocker?                   
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
    free(species);
}
/**************************************************************************************************************/
/*                              getHSP: get the Info in the XML file                                          */
/** Parametre : node : Node where we are now                                                                  */
/**           : name :name of the element                                                                     */
/**           : label : name we put in the json file                                                          */
/**           : query_length : length of the query                                                            */
/**************************************************************************************************************/
void getHSP(xmlNode *node, const char *name, char *label, int query_length)
{ // ajout d'un autre char *
    if (strcmp(name, (const char *)node->name) == 0)
    {

//récupération du tfrom
        if (strcmp(name, (const char *)"Hsp_hit-from") == 0)
        {
            t_from = atoi((const char *)xmlNodeGetContent(node));
            fprintf(output, "\t\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
//récupération du tto
        else if (strcmp(name, (const char *)"Hsp_hit-to") == 0)
        {
            t_to = atoi((const char *)xmlNodeGetContent(node));
            fprintf(output, "\t\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
        else if (strcmp(name, (const char *)"Hsp_align-len") == 0)
        {
//Ajout de la query cover
            int querycover = 100 * (t_to - t_from) / query_length;
            fprintf(output, "\t\t\t\t\"query-cover\" : \"%d\",\n", querycover);
            fprintf(output, "\t\t\t\t\"%s\" : \"%s\"\n\t\t}", label, xmlNodeGetContent(node));
        }
        else
        {
//élément habituel
            fprintf(output, "\t\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
    }
}
