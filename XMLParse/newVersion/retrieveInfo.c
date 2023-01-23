#include "retrieveInfo.h"

char name_hit[128];
char name_species[30];
char *buffer;
char taxoID[128], parentspecies[128], ranks[128], espece[128], lineage[1024];


char *makebuffer(FILE *file){
    long fileSize;
   
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    buffer = (char *) malloc((fileSize + 1) * sizeof(char));
    fread(buffer, sizeof(char), fileSize, file);

    buffer[fileSize] = '\0';
    return buffer;
}

/// @brief Retrieve the definition of query
/// @param node
void query_Def(xmlNode *node)
{

    const char *name;
    name = "BlastOutput_query-def";

    if (strcmp(name, (const char *)node->name) == 0)
    {
        // affichage
        fprintf(output, "{\n\t\"query-name\" : \"%s\",\n", xmlNodeGetContent(node));
    }
}

/// @brief Retrieve the length of the query
/// @param node
void query_Length(xmlNode *node)
{
    const char *name;
    name = "BlastOutput_query-len";

    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "\t\"query-length\" : \"%s\",\n\t\"hits\" :\n\t[\n", xmlNodeGetContent(node));
        // we initialise the variable query_length to get the query-cover
        query_length = xmlNodeGetContent(node);
    }
}

/// @brief Browe the XML file to get elements we want
/// @param fichier
/// @param mode
void blastOutPut_iteration(xmlDoc *fichier, char *mode, FILE *file) // ajout du file taxo
{
    makebuffer(file);
    xmlNode *node, *child, *root;
    /**********************************RECUP° DU PREMIER NOEUD******************************************/
    root = xmlDocGetRootElement(fichier);
    /********************************CHILD = SOUS-NOEUD DU ROOT ****************************************/
    child = root->children;

    for (node = child; node; node = node->next)
    {
        query_Def(node);
        query_Length(node);
    }
    const char *nodeName, *iteration;
    nodeName = "BlastOutput_iterations";
    /**********************************PARCOURS DES SOUS-NOEUD******************************************/
    for (node = child; node; node = node->next)
    {
        /***************************SI ON EST DANS BlastOutput_iterations************************************/
        if (strcmp(nodeName, (const char *)node->name) == 0)
        {
            xmlNode *childnode;
            /********************************SOUS-NOEUD DE BLASTOUPUT....***************************************/
            childnode = node->children;
            for (child = childnode; child; child = child->next)
            {
                /********************************ON ENTRE DANS ITERATION POUR***************************************/
                iteration = "Iteration";
                if (strcmp(iteration, (const char *)child->name) == 0)
                {
                    /********************************PARCOURS SOUS-NOEUD ITERATION**************************************/
                    hit_Iteration(child, mode);
                }
            }
        }
    }
}

/// @brief Retrieve HIT Node
/// @param node
/// @param mode
void hit_Iteration(xmlNode *node, char *mode)
{
    xmlNode *child;
    const char *iteration, *hit;
    /********************************CHILD = SOUS-NOEUD DU NODE ****************************************/
    child = node->children;
    iteration = "Iteration_hits";
    hit = "Hit";
    int i = 0;
    /***********************************PARCOURS DES SOUS-NOEUD*****************************************/
    for (node = child; node; node = node->next)
    {
        if (strcmp(iteration, (const char *)node->name) == 0)
        {
            xmlNode *childNode;
            /********************************CHILDNODE = SOUS-NOEUD DE CHILD************************************/
            childNode = node->children;
            for (child = childNode; child; child = child->next)
            {
                if (strcmp(hit, (const char *)child->name) == 0)
                {
                    i++;
                    /*************************************ENTRER DANS L'HSP*********************************************/
                    HSP_Enter(child, mode, hit_ID(child, mode), getSpecies(child)); // ajouter un autre paramètres!
                }
            }
        }
    }
}

/**
 * identifier le hit id (hit id / hit_accession)
 */

/// @brief Identification of the hit id
/// @param node
/// @param mode
/// @return hit id
char *hit_ID(xmlNode *node, char *mode)
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

/** @brief Retrieve the name of species from hit_def
** @param content
** @return name_species
*/
char *getSpecies(xmlNode *node)
{
    char *content;
    xmlNode *child;
    const char *name = "Hit_def";
    child = node->children;
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

/// @brief Enter to the HSP node
/// @param node
/// @param mode
/// @param hit_id
/// @param species
void HSP_Enter(xmlNode *node, char *mode, char *hit_id, char *species) // ajout du file taxo
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
            for (child = childNode; child; child = child->next)
            {
                /***************************NOEUD DANS LEQUEL ON SE TROUVE = HSP ?**********************************/
                if (strcmp(name2, (const char *)child->name) == 0)
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
                    // mettre le lineage ici
                    fprintf(output, "\t\t\t\"species\": [\n\t\t\t\t{\n");
                    // vérifie d'abord que la donnée est déjà stocké (pour ne pas reparcourir le tableau)
                    if (strcmp(species, espece) == 0)
                    {
                        // name
                        fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n", species);
                        // taxid 1
                        fprintf(output, "\t\t\t\t\t\"taxid\": \"%s\",\n", taxoID);
                        // parent 3
                        fprintf(output, "\t\t\t\t\t\"parent\": \"%s\",\n", parentspecies);
                        // rang 2
                        fprintf(output, "\t\t\t\t\t\"rank\": \"%s\",\n", ranks);
                        // fermeture
                        fprintf(output, "\t\t\t\t\t\"lineage\": \"%s\"\n", readTaxo(species)); // ajout du file
                        fprintf(output, "\t\t\t\t}\n\t\t\t],\n");
                    }
                    else if (strcmp(species, espece) != 0)
                    {
                        char *line = strtok(buffer, "\n");
                        char id[255], name[255], rank[255], parent[255];
                        int presence =0;
                        while (line != NULL)
                        {
                            sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", id, name, rank, parent);
                            if (strcmp(name, species) == 0)
                            {
                                presence =1;
                                fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n", name);
                                strcpy(espece, name);
                                // taxid 1
                                fprintf(output, "\t\t\t\t\t\"taxid\": \"%s\",\n", id);
                                strcpy(taxoID, id);
                                // parent 3
                                fprintf(output, "\t\t\t\t\t\"parent\": \"%s\",\n", parent);
                                strcpy(parentspecies, parent);
                                // rang 2
                                fprintf(output, "\t\t\t\t\t\"rank\": \"%s\",\n", rank);
                                strcpy(ranks, rank);
                                fprintf(output, "\t\t\t\t\t\"lineage\": \"%s\"\n", readTaxo(species));
                                strcpy(lineage, readTaxo(species));
                                // fermeture
                                fprintf(output, "\t\t\t\t}\n\t\t\t],\n");
                                break;
                            }
                            line = strtok(NULL, "\n");
                        }
                        if(presence ==0) {
                            fprintf(output, "\t\t\t\t\t\"name\":\"%s\"\n", species);
                            fprintf(output, "\t\t\t\t}\n\t\t\t],\n");
                        }
                    }

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

/// @brief Retrieve Element
/// @param node
/// @param name
/// @param label
void getHSP(xmlNode *node, const char *name, char *label)
{ // ajout d'un autre char *
    if (strcmp(name, (const char *)node->name) == 0)
    {
        /******************************Ajout de la query cover **************************************/
        if (strcmp(name, (const char *)"Hsp_align-len") == 0)
        {
            // on met la query cover ici !
            int querycover = 100 * (t_to - t_from) / query_length;
            fprintf(output, "\t\t\t\"query-cover\" : \"%d\",\n", querycover);
            fprintf(output, "\t\t\t\"%s\" : \"%s\"\n\t\t}", label, xmlNodeGetContent(node));
        }
        /********************************récupération du tfrom****************************/
        else if (strcmp(name, (const char *)"Hsp_hit_from") == 0)
        {
            t_from = xmlNodeGetContent(node);
            fprintf(output, "\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
        /********************************récupération du to****************************/
        else if (strcmp(name, (const char *)"Hsp_hit_to") == 0)
        {
            t_to = xmlNodeGetContent(node);
            fprintf(output, "\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
        else
        {
            fprintf(output, "\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
    }
}

char *retrieveParent(char *speciesID, char *lineage)
{

     char *line = strtok(strdup(buffer), "\n");
    char taxID[128], name[128], rank[128], parentID[128];
    char *lignee = (char *)malloc(2058);
    // on revient au début du fichier
    while (line != NULL)
    {
        sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);
        if (strcmp(speciesID, taxID) == 0)
        {
            strcat(lignee, name);
            strcat(lignee, "/");
            strcat(lignee, lineage);
            strcpy(lineage, lignee);
            retrieveParent(parentID, lineage);
        }
        line = strtok(NULL, "\n");
    }
    return lineage;
}

/// @brief Browse the buffer
/// @param species_name 
/// @return 
char *readTaxo(char *species_name)
{

    char *line = strtok(strdup(buffer), "\n");

    char taxID[128], name[128], rank[128], parentID[128];
    char *lineage = (char *)malloc(1024);
    char *finalResult = (char *)malloc(1024);
    
    // récupération de la premiere ligne

    while (line != NULL)
    {
        sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);
        //printf("on rentre ici \n");
        if (strcmp(name, species_name) == 0)
        {
           // printf("name is %s\n", name);
            if (strcmp(rank, "superkingdom") == 0)
            {
                finalResult = name;
                break;
            }
            else
            {
                strcpy(lineage, name);
                printf("on est là y le parent id est %s\n", parentID);
                // //récupération du parent puis on cherche le parent à l'aide de son taxid
                finalResult = retrieveParent(parentID, lineage);
                // break;
            }
        }
        line = strtok(NULL, "\n");
    }
    return finalResult;
}
