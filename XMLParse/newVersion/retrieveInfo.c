#include "retrieveInfo.h"
#include "blastInfo.h"
#include "lineage.h"
#include "parameters.h"


char name_hit[MIN_SIZE];
char name_species[32];
char taxoID[MIN_SIZE], parentspecies[MIN_SIZE], ranks[MIN_SIZE], espece[MIN_SIZE], lineage[MAX_SIZE]; // mettre 4096 à lineage
int query_length=0, t_from = 0, t_to = 0;



/// @brief Browse the XML FILE
/// @param fichier
/// @param mode
/// @param buffer
void blastOutPut_iteration(xmlDoc *fichier, char *mode, char *buffer)
{
    // Declare the three node allowing us to browse the XML FILE
    xmlNode *node, *child, *root;
    /**********************************GET THE FIRST NODE******************************************/
    root = xmlDocGetRootElement(fichier);
    /********************************CHILD = SUBNOD OF THE ROOT ****************************************/
    child = root->children;
    /*******************************WE LOOK FOR EACH NODE TO GET ***************************************/
    /******************************THE LENGTH AND THE DEF OF THE QUERY**********************************/
    char *speciesName=(char *)malloc(sizeof(char)*MIN_SIZE);
    int query_length=0;
    for (node = child; node; node = node->next)
    {
        //printf("Les noeuds sont %s\n", node->name);
        getBlastVersion(node);
        getBlastDB(node);
        getQueryDef(node);
        speciesName=retrieveDef(node);
        //printf("speciesName is %s \n", speciesName);
        getQueryLength(node, query_length);
    }
    //printf("l'espece est %s\n",getQuerySpeciesName(speciesName));
    displayQuerySpecies(speciesName, buffer);
    fprintf(output, "\t\t\"hits\": [\n\t\t {\n");  // We initialize it to BlastOutput_iteration
    const char *BLASTOUTPUT_NODE_NAME = "BlastOutput_iterations";
     /**********************************PATH OF SUBNODES******************************************/
    for (node = child; node; node = node->next)
    {
        /***************************CHECK IF WE'RE IN BLASTOUTPUT-ITERATIONS************************************/
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
                    node_Iteration(child, mode, buffer);
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
void node_Iteration(xmlNode *node, char *mode, char *buffer)
{
    xmlNode *child;
    const char *iteration = "Iteration_hits", *hit = "Hit";
    /********************************CHILD = SOUS-NOEUD DU NODE ****************************************/
    child = node->children;
    /***********************************PARCOURS DES SOUS-NOEUD*****************************************/
    for (node = child; node; node = node->next)
    {
        // check if we're on the node "iteration_hits"
        if (strcmp(iteration, (const char *)node->name) == 0)
        {
            /********************************CHILDNODE = SUBNODES OF CHILD************************************/
            xmlNode *childNode = node->children;
            // initialisation la structure ici ?
            SpeciesInfo *speciesInfo = (SpeciesInfo *)malloc(sizeof(SpeciesInfo));
            speciesInfo->next = NULL;
            speciesInfo->previous = NULL;
            for (child = childNode; child; child = child->next)
            {
                // check if we're on the node "iteration_hits"
                if (strcmp(hit, (const char *)child->name) == 0)
                {
                    /*************************************ENTRER DANS L'HSP*********************************************/
                    node_HSP(child, mode, getHitAccession(child, mode), getSpecies(child), buffer, speciesInfo); // ajouter un autre paramètre!
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

void node_HSP(xmlNode *node, char *mode, char *hit_id, char *species, char *buffer, SpeciesInfo *species_info) // ajout du file taxo
{
    xmlNode *child;
    const char *HIT_HSP_NAME = "Hit_hsps", *HSP_NAME = "Hsp";
    /********************************CHILD = SOUS-NOEUD DU NODE ****************************************/
    child = node->children;
    for (node = child; node; node = node->next)
    {
        if (strcmp(HIT_HSP_NAME, (const char *)node->name) == 0)
        {
            /*****************************CHILDNODE = SOUS-NOEUD DE NODE***************************************/
            xmlNode *childNode;
            childNode = node->children;
            for (child = childNode; child; child = child->next)
            {
                if (strcmp(HSP_NAME, (const char *)child->name) == 0)
                {
                    if (strcmp(name_hit, "") == 0)
                    {
                        fprintf(output, "\t\t{\n\t\t\t\"hit_accession\" : \"%s\",\n", hit_id);
                        strcpy(name_hit, hit_id);
                    }
                    else
                    {
                        fprintf(output, "\t\t,{\n\t\t\t\"hit_accession\" : \"%s\",\n", hit_id);
                    }

                    fprintf(output, "\t\t\t\"species\": [\n\t\t\t\t{\n");
                    // vérifie d'abord que la donnée est déjà stocké (pour ne pas reparcourir )
                    // parcourir la structure avec un while /if
                    // int find = 0;
                    // SpeciesInfo *current = species_info;
                    // while (current != NULL)
                    // {
                    //     if (strcmp(species, current->name) == 0)
                    //     {
                    //         fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n", current->name);
                    //         // taxid 1
                    //         fprintf(output, "\t\t\t\t\t\"taxid\": \"%s\",\n", current->id);
                    //         // parent 3
                    //         fprintf(output, "\t\t\t\t\t\"parent\": \"%s\",\n", current->parent);
                    //         // rang 2
                    //         fprintf(output, "\t\t\t\t\t\"rank\": \"%s\",\n", current->rank);
                    //         // fermeture
                    //         fprintf(output, "\t\t\t\t\t\"lineage\": \"%s\"\n", current->lineage); // ajout du file
                    //         fprintf(output, "\t\t\t\t}\n\t\t\t],\n");
                    //         // fprintf(output, "%s", current->displayInfo);
                    //         find = 1;
                    //         break;
                    //     }
                    //     current = current->next;
                    // }
                    // if (find == 0)
                    // {
                    //     char *line = strtok(strdup(buffer), "\n");
                    //     char id[255], name[255], rank[255], parent[255];
                    //     int presence = 0;

                    //     SpeciesInfo *fillInfo = (SpeciesInfo *)malloc(sizeof(SpeciesInfo));

                    //     while (line != NULL)
                    //     {
                    //         sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", id, name, rank, parent);
                    //         if (strcmp(name, species) == 0)
                    //         {
                    //             presence = 1;
                    //             fillInfo->previous = species_info;
                    //             fillInfo->next = NULL;

                    //             strcpy(fillInfo->name, name);
                    //             strcpy(fillInfo->id, id);
                    //             strcpy(fillInfo->rank, rank);
                    //             strcpy(fillInfo->parent, getParentName(buffer, parent));
                    //             char *myTaxo = readTaxoFile(buffer, fillInfo->name);
                    //             strcpy(fillInfo->lineage, myTaxo);
                    //             free(myTaxo);
                    //             fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n", fillInfo->name);
                    //             // taxid 1
                    //             fprintf(output, "\t\t\t\t\t\"taxid\": \"%s\",\n", fillInfo->id);
                    //             // parent 3
                    //             fprintf(output, "\t\t\t\t\t\"parent\": \"%s\",\n", fillInfo->parent);
                    //             // rang 2
                    //             fprintf(output, "\t\t\t\t\t\"rank\": \"%s\",\n", fillInfo->rank);
                    //             // fermeture
                    //             fprintf(output, "\t\t\t\t\t\"lineage\": \"%s\"\n", fillInfo->lineage); // ajout du file
                    //             fprintf(output, "\t\t\t\t}\n\t\t\t],\n");
                    //             species_info = fillInfo;
                    //             break;
                    //         }
                    //         line = strtok(NULL, "\n");
                    //     }
                    //     if (presence == 0)
                    //     {
                    //         fprintf(output, "\t\t\t\t\t\"name\":\"%s\"\n", species);
                    //         fprintf(output, "\t\t\t\t}\n\t\t\t],\n");
                    //     }
                    // }

                    /*****************************LASTCHILD = SOUS-NOEUD DE CHILD***************************************/
                    xmlNode *lastchild;
                    lastchild = child->children;
                    for (childNode = lastchild; childNode; childNode = childNode->next)
                    {
                        /*****************************************MODE BRONZE***********************************************/
                        if (strcmp(mode, "bronze") == 0)
                        {
                            getHSP(childNode, "Hsp_num", "number of hit");
                            getHSP(childNode, "Hsp_identity", "identity");
                            getHSP(childNode, "Hsp_align-len", "align_len");
                            getHSP(childNode, "Hsp_gaps", "gaps");
                            getHSP(childNode, "Hsp_query-from", "query_from");
                            getHSP(childNode, "Hsp_query-to", "query_to");
                            getHSP(childNode, "Hsp_hit-from", "target_from");
                            getHSP(childNode, "Hsp_hit-to", "target_to");
                            getHSP(childNode, "Hsp_positive", "positive");
                            getHSP(childNode, "Hsp_evalue", "evalue");
                            getHSP(childNode, "Hsp_score", "score");
                            getHSP(childNode, "Hsp_bit-score", "bitscore");
                        }
                        /****************************************MODE SILVER************************************************/
                        else if (strcmp(mode, "silver") == 0)
                        {
                            // getHSP(childNode, "Hsp_num", "number of hit");
                            // getHSP(childNode, "Hsp_identity", "identity");
                            // getHSP(childNode, "Hsp_align-len", "align_len");
                            // getHSP(childNode, "Hsp_gaps", "gaps");
                            // getHSP(childNode, "Hsp_query-from", "query_from");
                            // getHSP(childNode, "Hsp_query-to", "query_to");
                            // getHSP(childNode, "Hsp_hit-from", "target_from");
                            // getHSP(childNode, "Hsp_hit-to", "target_to");
                            // getHSP(childNode, "Hsp_positive", "positive");
                            // getHSP(childNode, "Hsp_evalue", "evalue");
                            // getHSP(childNode, "Hsp_score", "score");
                            // getHSP(childNode, "Hsp_bit-score", "bitscore");
                        }
                        /*****************************************MODE GOLD*************************************************/
                        else if (strcmp(mode, "gold") == 0)
                        {
                            getHSP(childNode, "Hsp_num", "number of hit");
                            getHSP(childNode, "Hsp_identity", "identity");
                            getHSP(childNode, "Hsp_align-len", "align_len");
                            getHSP(childNode, "Hsp_gaps", "gaps");
                            getHSP(childNode, "Hsp_query-from", "query_from");
                            getHSP(childNode, "Hsp_query-to", "query_to");
                            getHSP(childNode, "Hsp_hit-from", "target_from");
                            getHSP(childNode, "Hsp_hit-to", "target_to");
                            getHSP(childNode, "Hsp_positive", "positive");
                            getHSP(childNode, "Hsp_evalue", "evalue");
                            getHSP(childNode, "Hsp_score", "score");
                            getHSP(childNode, "Hsp_bit-score", "bitscore");
                            //ajout des motifs extraits
                        }
                        /****************************************MODE PERSO*************************************************/
                        else
                        {
                            printf("nothing\n");
                        }
                    }
                    fprintf(output, "\n");
                }
            }
        }
    }
}

void getHSP(xmlNode *node, const char *name, char *label)
{ // ajout d'un autre char *
    if (strcmp(name, (const char *)node->name) == 0)
    {
        /******************************Ajout de la query cover **************************************/

        /********************************récupération du tfrom****************************/
        if (strcmp(name, (const char *)"Hsp_hit-from") == 0)
        {
            t_from = atoi((const char *)xmlNodeGetContent(node));
            fprintf(output, "\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
        /********************************récupération du to****************************/
        else if (strcmp(name, (const char *)"Hsp_hit-to") == 0)
        {
            t_to = atoi((const char *)xmlNodeGetContent(node));
            fprintf(output, "\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
        else if (strcmp(name, (const char *)"Hsp_align-len") == 0)
        {
            // on met la query cover ici !
            int querycover = 100 * (t_to - t_from) / query_length;
            fprintf(output, "\t\t\t\"query-cover\" : \"%d\",\n", querycover);
            fprintf(output, "\t\t\t\"%s\" : \"%s\"\n\t\t}", label, xmlNodeGetContent(node));
        }
        else
        {
            fprintf(output, "\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
    }
}
