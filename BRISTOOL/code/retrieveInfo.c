/**************************************************************************************************************/
/*                                        RETRIEVEINFO.C                                                      */
/** MAIN FUNCTION FOR GETTING THE JSON FILE                                                                   */
/** Global variable : name_hit                                                                                */
/**                   tfrom, tto :                                                                            */
/**************************************************************************************************************/
#include "retrieveInfo.h"
#include "blastInfo.h"
#include "lineage.h"
#include "makebuffer.h"
#include "parameters.h"
#include "fillStructure.h"
#include "extractMotifs.h"
#include "XMLtoBLASTP.h"

char name_hit[MIN_SIZE];
int size_struct=0; // voir où on peut le mettre en local au lieu de glo

/**************************************************************************************************************/
/*                       blastOutPut_iteration: allowing us to enter to the first node                        */
/** Parametre : fichier : XML FILE                                                                            */
/**             mode : Bronze, Silver, Gold                                                                   */
/**             buffer : taxonomy.dat                                                                         */
/**************************************************************************************************************/
void blastOutPut_iteration(xmlDoc *fichier, char *mode, char *buffer, char tabInfo[13][2][20])
{
    xmlNode *node, *root = xmlDocGetRootElement(fichier), *child = root->children;

    char speciesName[MIN_SIZE];
    int query_length = 0;
    //get the blast version and the database
    for (node = child; node; node = node->next)
    {
        blastVersion(node);
        blastDB(node);
    }  
    //fill the structure and create the HashMap
    SpeciesInfo *speciesInfo = fillStructure(buffer);
    FillSpeciesInfo *fillInfo = (FillSpeciesInfo *)malloc(sizeof(FillSpeciesInfo)*MAXI_SIZE);
    Hashmap *hashmap = createHashMap(buffer);

   //condition for gold mode : convert the xml to blastp file
    if(strcmp(mode, "gold")==0){
        char *database = (char*)malloc(sizeof(char));
        char *blastInfo=getInfoBlast(child, database);
        convertToBlastP(fichier, child, blastInfo, database);
        free(database);
        free(blastInfo);
    }
  
   fprintf(output, "\t\"blast_output\":[\n"); 

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
                    char *iteration_num;
                    for(xmlNode *childnode=child->children; childnode; childnode=childnode->next){
                        // get the query def
                        if(strcmp("Iteration_query-def", (const char *)childnode->name) == 0)
                        {
                            char *query_def = (char *)xmlNodeGetContent(childnode);
                            strcpy(speciesName, query_def);
                            fprintf(output, " \t{\n\t\t\"query-name\" : \"%s\",\n", query_def);
                        }
                        // get the iteration number
                        else if(strcmp("Iteration_iter-num", (const char *)childnode->name) == 0){
                            iteration_num=(char *)xmlNodeGetContent(childnode);
                        }
                        // get the query length
                        else if (strcmp("Iteration_query-len", (const char *)childnode->name) == 0)
                        {
                            fprintf(output, "\t\t\"query-length\" : \"%s\",\n", xmlNodeGetContent(childnode));
                            query_length = atoi((const char *)xmlNodeGetContent(childnode));
                        }
                    }
                    //display the species name
                    INFO("Iteration : %s", iteration_num);
                    displayQuerySpecies(speciesName);
                    node_Iteration(child, mode, speciesInfo, query_length, fillInfo, hashmap,tabInfo, iteration_num); //ajout de la table d'information
                }
            }
        }
    }
    //free the memory
    free(hashmap);
    free(fillInfo);
    free(speciesInfo);
    free(buffer);
    fseek(output,-2, SEEK_END);
    fprintf(output, "\n ]\n}");
}

/**************************************************************************************************************/
/*                           Node_Iteration: allowing us to enter to the first node                           */
/** Parametre : node : Node where we are now                                                                  */
/**             query_length :get the length                                                                  */
/**             buffer : taxonomy.dat                                                                         */
/**************************************************************************************************************/
void node_Iteration(xmlNode *node, char *mode, SpeciesInfo *speciesInfo, int query_length, FillSpeciesInfo *fillInfo, Hashmap *hashmap, char tabInfo[13][2][20], char *iteration_num)
{
    fprintf(output, "\t\t\"hits\": [\n ");
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
                    node_HSP(child, mode, query_length, speciesInfo, fillInfo, hashmap, tabInfo); //ajout de la hashmap ici
                    name_hit[0]='\0';
                }
            }
        }
    }
    fseek(output, -2, SEEK_END);    
    fprintf(output, "\n\t\t],\n");
    //add motifs if the mode is gold
    if(strcmp(mode, "gold")==0){
        char blastp[MIN_SIZE] = BLAST_FILE;
        strcat(blastp,iteration_num);
        char result_file[MIN_SIZE];
        strcpy(result_file,blastp);
        strcat(blastp,".blastp\0");
        FILE *f = fopen(blastp, "r");
        if(f!=NULL){
            fclose(f);
        }
        else{
            fprintf(stdout, "No motifs\n");
        }
        char command[MIN_SIZE] = BALLAST_ACCESS ;
        strcat(command," -p ");
        strcat(command, blastp);
        strcat(command, " -o ");
        strcat(command, result_file);
        system(command);
        strcat(result_file,".motifs");
        char *blastBuffer = makebuffer(result_file);
        char *extractmotif=getMotifs(blastBuffer);
        fprintf(output,"%s\n\t\t}\n\t],\n",extractmotif);
    }
    else{
        //end of the iteration)
        fseek(output, -2,SEEK_END);
        fprintf(output,"\n\t},\n");
    }
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
/**             fragment : if the content is a fragment or not                                                */
/**************************************************************************************************************/
char *getSpecies(xmlNode *node, char *fragment)
{

    char *content;
    const char *name = "Hit_def";
    xmlNode *child = node->children;
    for (node = child; node; node = node->next)
    {
        if (strcmp(name, (const char *)node->name) == 0)
        {
            content = (char *)xmlNodeGetContent(node);
            break;
        }
    }
    //vérifier si content contient le mot "fragment" ou le mot "partial"
    if (strstr(content, "fragment") != NULL || strstr(content, "partial") != NULL ||strstr(content, "Fragment")!=NULL)
    {
       fragment="yes";
    }

    int debut = 0, fin = 0;
    //d'abord avec OS et OX
    for (int i = 0; i < strlen(content); i++)
    {
        if (content[i] == 'O' && content[i+1] == 'S' && content[i+2] == '=')
        {
            debut = i+2;
        }
        else if (content[i] == 'O' && content[i+1] == 'X' && content[i+2] == '=')
        {
            fin = i-1;
            break;
        }
       
    }
    if(debut !=0 && fin !=0){
        char name_species[MAX_SIZE];
        int k = 0;
        for (int j = debut + 1; j < fin; ++j)
        {
            name_species[k] = content[j];
            k++;
        }        
        name_species[k] = '\0';
        return strdup(name_species);
    }
    else {
        fin =0;
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
void node_HSP(xmlNode *node, char *mode,int query_length, SpeciesInfo *speciesInfo, FillSpeciesInfo *fillInfo, Hashmap *hashmap, char tabInfo[13][2][20]) // ajout de la hashmap
{
    char *fragment ="no";
    char *hit_id = getHitAccession(node),*species = getSpecies(node, fragment);
    xmlNode *child = node->children;
    const char *nameHitHspNode = "Hit_hsps", *nameHspNode = "Hsp";

    for (node = child; node; node = node->next)
    {
        if (strcmp(nameHitHspNode, (const char *)node->name) == 0)
        {
            xmlNode *childNode = node->children;
            for (child = childNode; child; child = child->next)
            {
                if(strcmp((char *)child->name, "text")!=0){
                    //compare the name of the node
                    if (strcmp(nameHspNode, (char *)child->name) == 0)
                    {
                        //verify if we haven't the species
                        if (strcmp(name_hit, "") == 0)
                        {
                            strcpy(name_hit, hit_id);
                        }

                       fprintf(output, "\t\t\t{\n\t\t\t\t\"hit-accession\" : \"%s\",\n\t\t\t\t\"fragment\" : \"%s\",\n\t\t\t\t\"species\" : {\n", hit_id, fragment);

                        //we got the id of the species
                        int taxId = get(hashmap, species);
                        //we verify if the second structure is empty
                        if(size_struct==0){
                            //we add the first species and its informations
                            fillInfo[0].name=speciesInfo[taxId].name;
                            fillInfo[0].id=taxId;
                            fillInfo[0].rank=speciesInfo[taxId].rank;
                            
                            fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n",fillInfo[0].name);
                            fprintf(output, "\t\t\t\t\t\"taxid\":\"%u\",\n", fillInfo[0].id);
                            fprintf(output,"\t\t\t\t\t\"rank\":\"%s\"",fillInfo[0].rank);
                            //we add the lineage of the species 
                            if(strcmp(tabInfo[0][1], "1")==0){
                                fillInfo[0].lineage=createLineage(speciesInfo, species, hashmap);
                                fprintf(output,",\n%s\n",fillInfo[0].lineage);
                            }
                            size_struct=1;
                        }
                        //if the structure is not empty
                        else if(size_struct>0){
                            //first we check if we already have the species and its informations
                            int check=0;
                            for(int i=0; i<size_struct; i++){
                                if(fillInfo[i].id==taxId){
                                    fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n",fillInfo[i].name);
                                    fprintf(output, "\t\t\t\t\t\"taxid\":\"%u\",\n",fillInfo[i].id);
                                    fprintf(output,"\t\t\t\t\t\"rank\":\"%s\"",fillInfo[i].rank);
                                    if(strcmp(tabInfo[0][1], "1")==0){
                                        fprintf(output,",\n%s",fillInfo[i].lineage );
                                    }
                                check=1;
                                break;
                                }
                            }
                            if(check == 0){
                                //if we don't have the species and its informations
                                if(taxId!=-1){
                                    for (int i = 0; i < SPECIES_SIZE - 1; i++)
                                    {
                                        fillInfo[size_struct].name=speciesInfo[taxId].name;
                                        fillInfo[size_struct].id=taxId;
                                        fillInfo[size_struct].rank=speciesInfo[taxId].rank;

                                        fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n",fillInfo[size_struct].name);
                                        fprintf(output, "\t\t\t\t\t\"taxid\":\"%u\",\n", fillInfo[size_struct].id);
                                        fprintf(output,"\t\t\t\t\t\"rank\":\"%s\"", fillInfo[size_struct].rank);
                                        if(strcmp(tabInfo[0][1], "1")==0){
                                            fillInfo[size_struct].lineage=createLineage(speciesInfo, species, hashmap); //ici on met la hashmap
                                            fprintf(output,",\n%s",fillInfo[size_struct].lineage ); //parent+lignée
                                        }
                                        size_struct++;
                                        break;
                                    }
                                }
                                //if the species is not in the taxonomy.dat
                                else{
                                    //we display just the name of the species.
                                    fprintf(output, "\t\t\t\t\t\"name\":\"%s\"",species);
                                    WARNING("Species : %s not found in taxonomy.dat", species);
                                }
                            }
                        }
                        fprintf(output,"\n\t\t\t\t},");
                    }
                    /*****************************LASTCHILD = SOUS-NOEUD DE CHILD***************************************/

                    xmlNode *lastchild = child->children;
                    int t_from=0, t_to=0;
                    //verify the mode we choose to display informations
                    //the mode bronze display the same informations than the mode gold
                    if(strcmp(mode, "bronze")==0 || strcmp(mode, "gold")==0){   
                        for (childNode = lastchild; childNode; childNode = childNode->next)
                        {  
                            for(int i =1; i<13; i++){
                                getHSP(childNode,tabInfo[i][0]);
                                //get the value of t_from
                                 if(strcmp("Hsp_hit-from", (char *)childNode->name)==0){
                                    t_from=atoi((char *)xmlNodeGetContent(childNode));
                                }
                                //get the value of t_to
                                if(strcmp("Hsp_hit-to", (char *)childNode->name)==0){
                                    t_to=atoi((char *)xmlNodeGetContent(childNode));
                                }                                           
                            }
                        }
                        //display the query cover 
                         if(strcmp(tabInfo[12][0], "Query_cover")==0 ){
                            int querycover = 100 * (t_to-t_from)/query_length;
                            fprintf(output, "\n\t\t\t\t\"query-cover\" : \"%d\",", querycover);
                        }   
                    }
                    //the mode silver 
                    else if (strcmp(mode, "silver")==0){
                        for (childNode = lastchild; childNode; childNode = childNode->next)
                        {  
                            //we brows the tabInfo to display the informations
                            for(int i =1; i<13; i++){
                                if(strcmp(tabInfo[i][0], (char *)childNode->name)==0){
                                    if(strcmp(tabInfo[i][1],"1")==0){
                                        getHSP(childNode,tabInfo[i][0]);
                                    }
                                    if(strcmp("Hsp_hit-from", (char *)childNode->name)==0){
                                        t_from=atoi((char *)xmlNodeGetContent(childNode));
                                    }
                                    if(strcmp("Hsp_hit-to", (char *)childNode->name)==0){
                                        t_to=atoi((char *)xmlNodeGetContent(childNode));
                                    }
                                }
                            }
                        }   
                        //display the query cover if the user has specified it
                        if(strcmp(tabInfo[12][0], "Query_cover")==0 && strcmp(tabInfo[12][1], "1")){
                            int querycover = 100 * (t_to-t_from)/query_length;
                            fprintf(output, "\n\t\t\t\t\"query-cover\" : \"%d\",", querycover);
                        }
                        
                    }
                    //we delete the last comma
                    fseek(output,-1,SEEK_END);
                    fprintf(output,"\n\t\t\t},\n");
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
/**************************************************************************************************************/
void getHSP(xmlNode *node, const char *name)
{ 
    char label[MIN_SIZE];
    strcpy(label, name+4);  
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "\n\t\t\t\t\"%s\" : \"%s\",", label, xmlNodeGetContent(node));
    }
}
