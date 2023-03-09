/**************************************************************************************************************/
/*                                        RETRIEVEINFO.C                                                      */
/** MAIN FUNCTION FOR GETTING THE JSON FILE                                                                   */
/** Global variable : name_hit                                                                                */
/**                   tfrom, tto :                                                                            */
/**************************************************************************************************************/
#include "retrieveInfo.h"
#include "blastInfo.h"
#include "lineage.h"
// #include "hashmap.h"
#include "parameters.h"
#include "fillStructure.h"

char name_hit[MIN_SIZE];
int t_from = 0, t_to = 0, size_struct=0; // voir où on peut le mettre en local au lieu de glo

/**************************************************************************************************************/
/*                       blastOutPut_iteration: allowing us to enter to the first node                        */
/** Parametre : fichier : XML FILE                                                                            */
/**             mode : Bronze, Silver, Gold                                                                   */
/**             buffer : taxonomy.dat                                                                         */
/**************************************************************************************************************/
void blastOutPut_iteration(xmlDoc *fichier, char *mode, char *buffer)
{
    // Declare the three node allowing us to browse the XML FILE
    xmlNode *node, *root = xmlDocGetRootElement(fichier), *child = root->children;
    // WE LOOK FOR EACH NODE TO GET THE LENGTH, THE DEF OF THE QUERY, THE DEFINITION OF BLAST
    char speciesName[MIN_SIZE];
    int query_length = 0;
    for (node = child; node; node = node->next)
    {
        getBlastVersion(node);
        getBlastDB(node);
    }  
    //remplissage de la structure speciesInfo
    SpeciesInfo *speciesInfo = fillStructure(buffer);
    FillSpeciesInfo *fillInfo = (FillSpeciesInfo *)malloc(sizeof(FillSpeciesInfo)*MAXI_SIZE); //~8000
    //faire appel a la hashmap ici (remplissage une fois)
    Hashmap *hashmap = createHashMap(buffer);
    fprintf(output, "\t\"blast_output\":[\n");
    // WE DISPLAY THE INFOS OF THE SPECIES
    const char *BLASTOUTPUT_NODE_NAME = "BlastOutput_iterations";
    // PATH OF SUBNODES
    for (node = child; node; node = node->next)
    {
        // CHECK IF WE'RE IN BLASTOUTPUT-ITERATIONS
        if (strcmp(BLASTOUTPUT_NODE_NAME, (const char *)node->name) == 0)
        {
            xmlNode *childnode = node->children;
            // BLASTOUTPUT SUBNODES
            for (child = childnode; child; child = child->next)
            {
                // we initialize ITERATION :it's the beginning of blast analysis
                const char *ITERATION = "Iteration";
                if (strcmp(ITERATION, (const char *)child->name) == 0)
                {
                    // ITERATION SUBNODES
                    xmlNode *childrenNode = child->children;
                    for(childnode=childrenNode; childnode; childnode=childnode->next){
                        getQueryDef(childnode, speciesName);
                        if (strcmp("Iteration_query-len", (const char *)childnode->name) == 0)
                        {
                            fprintf(output, "\t\t\"query-length\" : \"%s\",\n", xmlNodeGetContent(childnode));
                            query_length = atoi((const char *)xmlNodeGetContent(childnode));
                        }
                    }
                    displayQuerySpecies(speciesName);
                    node_Iteration(child, mode, speciesInfo, query_length, fillInfo, hashmap); //ajout de la hashmap
                }
            }
        }
    }
    free(hashmap);
    free(buffer);
}

/**************************************************************************************************************/
/*                           Node_Iteration: allowing us to enter to the first node                           */
/** Parametre : fichier : XML FILE                                                                            */
/**             node : Node where we are now                                                                  */
/**             query_length :get the length                                                                  */
/**             buffer : taxonomy.dat                                                                         */
/**************************************************************************************************************/
void node_Iteration(xmlNode *node, char *mode, SpeciesInfo *speciesInfo, int query_length, FillSpeciesInfo *fillInfo, Hashmap *hashmap)
{
    fprintf(output, "\t\t\"hits\": [\n");
    // CHILD = SOUS-NOEUD DU NODE
    xmlNode *child = node->children;
    const char *iteration = "Iteration_hits", *hit = "Hit";

    for (node = child; node; node = node->next)
    {
        // check if we're on the node "iteration_hits"
        if (strcmp(iteration, (const char *)node->name) == 0)
        {
            // CHILDNODE = SUBNODES OF CHILD
            xmlNode *childNode = node->children;
            for (child = childNode; child; child = child->next)
            {
                // check if we're on the node "iteration_hits"
                if (strcmp(hit, (const char *)child->name) == 0)
                {
                    //HSP NODE
                    node_HSP(child, mode, query_length, speciesInfo, fillInfo, hashmap); //ajout de la hashmap ici
                }
            }
        }
    }
    fprintf(output, "\t\t]\n\t },\n");
}

/**************************************************************************************************************/
/*                           getHitAccession: like the name of the function ...                               */
/** Parametre : node : XML FILE                                                                               */
/**             mode : Bronze, Silver, Gold                                                                   */
/**************************************************************************************************************/
char *getHitAccession(xmlNode *node)
{
    char *hit_id = NULL;
    const char *name = "Hit_accession";
    xmlNode *child = node->children;
    for (node = child; node; node = node->next)
    {
        if (strcmp(name, (const char *)node->name) == 0)
        {
            hit_id = (char *)xmlNodeGetContent(node);
            return hit_id;
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
    
    int debut = 0, fin = 0;
    for (int i = 0; i < strlen(content); i++)
    {
        if (content[i] == '[')
        {
            debut = i;
        }
        else if (content[i] == ']')
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
void node_HSP(xmlNode *node, char *mode,int query_length, SpeciesInfo *speciesInfo, FillSpeciesInfo *fillInfo, Hashmap *hashmap) // ajout de la hashmap
{
    char *hit_id = getHitAccession(node),*species = getSpecies(node);
    // CHILD = SOUS-NOEUD DU NODE
    xmlNode *child = node->children;
    const char *name = "Hit_hsps", *name2 = "Hsp";
    for (node = child; node; node = node->next)
    {
        if (strcmp(name, (const char *)node->name) == 0)
        {
            // CHILDNODE = SOUS-NOEUD DE NODE
            xmlNode *childNode = node->children;
            for (child = childNode; child; child = child->next)
            {
                // NOEUD DANS LEQUEL ON SE TROUVE = HSP
                if (strcmp(name2, (char *)child->name) == 0)
                {
                    if (strcmp(name_hit, "") == 0)
                    {
                        fprintf(output, "\t\t\t{\n\t\t\t\t\"hit_accession\" : \"%s\",\n", hit_id);
                        strcpy(name_hit, hit_id);
                    }
                    else
                    {
                        fprintf(output, "\t\t\t,{\n\t\t\t\t\"hit_accession\" : \"%s\",\n", hit_id);
                    }

                    fprintf(output, "\t\t\t\t\"species\" : {\n");
                    //1e ajout dans la deuxieme structure
                    if(size_struct==0){
                        for (int i = 0; i < SPECIES_SIZE - 1; i++)
                        {
                            if (strcmp(speciesInfo[i].name, species) == 0)
                            {
                                fillInfo[0].name=speciesInfo[i].name;
                                fillInfo[0].id=i;
                                fillInfo[0].rank=speciesInfo[i].rank;
                                fillInfo[0].lineage=createLineage(speciesInfo, species, hashmap);

                                fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n",fillInfo[0].name);
                                fprintf(output, "\t\t\t\t\t\"taxid\":\"%u\",\n", fillInfo[0].id);
                                fprintf(output,"\t\t\t\t\t\"rank\":\"%s\",\n",fillInfo[0].rank);
                                fprintf(output,"%s\n",fillInfo[0].lineage);
                            
                                size_struct=1;
                                break;
                            }
                        }
                    }
                    //parcours de la deuxieme structure
                   else if(size_struct>0){
                        int check=0;
                        for(int i=0; i<size_struct; i++){
                                if(strcmp(fillInfo[i].name,species)==0){
                                    fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n",fillInfo[i].name);
                                    fprintf(output, "\t\t\t\t\t\"taxid\":\"%u\",\n",fillInfo[i].id);
                                    fprintf(output,"\t\t\t\t\t\"rank\":\"%s\",\n",fillInfo[i].rank);
                                    fprintf(output,"%s",fillInfo[i].lineage );
                                check=1;
                                break;
                            }
                        }
                        if(check == 0){
                            //on ajoute les infos à la position size_struct et on modifie la valeur de size_struct
                            for (int i = 0; i < SPECIES_SIZE - 1; i++)
                            {
                                if (strcmp(speciesInfo[i].name, species) == 0)
                                {
                                    fillInfo[size_struct].name=speciesInfo[i].name;
                                    fillInfo[size_struct].id=i;
                                    fillInfo[size_struct].rank=speciesInfo[i].rank;
                                    fillInfo[size_struct].lineage=createLineage(speciesInfo, species, hashmap); //ici on met la hashmap

                                    fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n",fillInfo[size_struct].name);
                                    fprintf(output, "\t\t\t\t\t\"taxid\":\"%u\",\n", fillInfo[size_struct].id);
                                    fprintf(output,"\t\t\t\t\t\"rank\":\"%s\",\n", fillInfo[size_struct].rank);
                                   fprintf(output,"%s",fillInfo[size_struct].lineage ); //parent+lignée
                                    size_struct++;
                                    break;
                                }
                            }
                        }
                    }
                    fprintf(output,"\n\t\t\t\t},\n");
                }
                /*****************************LASTCHILD = SOUS-NOEUD DE CHILD***************************************/
                xmlNode *lastchild = child->children;
                for (childNode = lastchild; childNode; childNode = childNode->next)
                {  
                    /*****************************************MODE BRONZE && GOLD***********************************************/
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
                    /*****************************************MODE SILVER***********************************************/
                }
            }
            fprintf(output, "\n");
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
        // récupération du tfrom
        if (strcmp(name, (const char *)"Hsp_hit-from") == 0)
        {
            t_from = atoi((const char *)xmlNodeGetContent(node));
            fprintf(output, "\t\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
        // récupération du tto
        else if (strcmp(name, (const char *)"Hsp_hit-to") == 0)
        {
            t_to = atoi((const char *)xmlNodeGetContent(node));
            fprintf(output, "\t\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
        else if (strcmp(name, (const char *)"Hsp_align-len") == 0)
        {
            // Ajout de la query cover
            int querycover = 100 * (t_to - t_from) / query_length;
            fprintf(output, "\t\t\t\t\"query-cover\" : \"%d\",\n", querycover);
            fprintf(output, "\t\t\t\t\"%s\" : \"%s\"\n\t\t\t}", label, xmlNodeGetContent(node));
        }
        else
        {
            // élément habituel
            fprintf(output, "\t\t\t\t\"%s\" : \"%s\",\n", label, xmlNodeGetContent(node));
        }
    }
}
