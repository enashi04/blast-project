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
#include "pretty.h"

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
    //json_content
    char *json_content = (char *)malloc(sizeof(char) * MAXI_SIZE+1);
    //get the blast version and the database
    for (node = child; node; node = node->next)
    {
        blastVersion(node, json_content);
        blastDB(node,json_content);
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
  
   strcat(json_content,"\"blast-output\":[");

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
                            strcat(json_content, " {\"query-name\" : \"");
                            strcat(json_content,query_def);
                            strcat(json_content, "\",");

                        }
                        // get the iteration number
                        else if(strcmp("Iteration_iter-num", (const char *)childnode->name) == 0){
                            iteration_num=(char *)xmlNodeGetContent(childnode);
                        }
                        // get the query length
                        else if (strcmp("Iteration_query-len", (const char *)childnode->name) == 0)
                        {
                            strcat(json_content, "\"query-length\" : ");
                            strcat(json_content, (char *)xmlNodeGetContent(childnode));
                            strcat(json_content, ",");
                            query_length = atoi((const char *)xmlNodeGetContent(childnode));
                        }
                    }
                    //display the species name
                    INFO("Iteration : %s", iteration_num);
                    displayQuerySpecies(speciesName, json_content);
                    char *content = node_Iteration(child, mode, speciesInfo, query_length, fillInfo, hashmap,tabInfo, iteration_num); //ajout de la table d'information
                    int len = strlen(content);
                    //printf("la longeuur de node iteration est %u\n",len);
                    json_content = (char *)realloc(json_content, sizeof(char) * (len + strlen(json_content) + 2));
                    strcat(json_content, content);
                }
            }
        }
    }
    //free the memory
    //free(json_content);
    free(hashmap);
    free(fillInfo);
    free(speciesInfo);
    free(buffer);
    int json_len = strlen(json_content);
    json_content[json_len-1]='\0';
    strcat(json_content, "]}");

    prettier(json_content);
   
}

/**************************************************************************************************************/
/*                           Node_Iteration: allowing us to enter to the first node                           */
/** Parametre : node : Node where we are now                                                                  */
/**             query_length :get the length                                                                  */
/**             buffer : taxonomy.dat                                                                         */
/**************************************************************************************************************/
char *node_Iteration(xmlNode *node, char *mode, SpeciesInfo *speciesInfo, int query_length, FillSpeciesInfo *fillInfo, Hashmap *hashmap, char tabInfo[13][2][20], char *iteration_num)
{
    //mettre cette fonction en char aussi je pense 
    char *json_content = (char *)malloc(sizeof(char) * MAXI_SIZE+1);
    strcat(json_content,"\"hits \":[ ");


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
                    char *contentHSP = node_HSP(child, mode, query_length, speciesInfo, fillInfo, hashmap, tabInfo); 
                    //longueur de content
                    int len_content = strlen(contentHSP);
                    //réallouer la mémoire
                    json_content = realloc(json_content, sizeof(char) * (len_content + strlen(json_content) + 1));
                    //concaténer
                    strcat(json_content, contentHSP);
                    //faire un strcat et transformer node_HSP en char *
                    name_hit[0]='\0';
                }
            }
        }
    }

    //delete the last comma
    int len_content = strlen(json_content);
    json_content[len_content - 1] = '\0';
    strcat(json_content,"],");
    //fseek(output, -2, SEEK_END);    
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

        //fprintf(output,"\n\t},\n");
        len_content=strlen(json_content);
        json_content[len_content-1]='\0';
        strcat(json_content,"},");
        // printf("json_content = %s\n", json_content);
    }
    return json_content;
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
       fragment="true";
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
char *node_HSP(xmlNode *node, char *mode,int query_length, SpeciesInfo *speciesInfo, FillSpeciesInfo *fillInfo, Hashmap *hashmap, char tabInfo[13][2][20]) // ajout de la hashmap
{
    char *fragment ="false";
    char *hit_id = getHitAccession(node),*species = getSpecies(node, fragment);
    xmlNode *child = node->children;
    const char *nameHitHspNode = "Hit_hsps", *nameHspNode = "Hsp";
    char contentHSP[SIZE_PLUS];
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
                        strcpy(contentHSP,"{\"hit-accession\" : \"");
                        strcat(contentHSP, hit_id);
                        strcat(contentHSP, "\",\"fragment\" : ");
                        strcat(contentHSP, fragment);
                        strcat(contentHSP, ",\"species\" : {");
                        //we got the id of the species
                        int taxId = get(hashmap, species);

                        //we verify if the second structure is empty
                        if(size_struct==0){
                            //we add the first species and its informations
                            fillInfo[0].name=speciesInfo[taxId].name;
                            fillInfo[0].id=taxId;
                            fillInfo[0].rank=speciesInfo[taxId].rank;
                             char string_id[MIN_SIZE];
                            sprintf(string_id, "%d", taxId);

                            strcat(contentHSP, "\"name\": \"");
                            strcat(contentHSP, fillInfo[0].name);
                            strcat(contentHSP, "\",\"taxid\":");
                            strcat(contentHSP, string_id);
                            strcat(contentHSP, ",\"rank\":\"");
                            strcat(contentHSP, fillInfo[0].rank);
                            strcat(contentHSP, "\",");

                            //we add the lineage of the species 
                            if(strcmp(tabInfo[0][1], "1")==0){
                                
                                fillInfo[0].lineage=createLineage(speciesInfo, species, hashmap);
                                strcat(contentHSP, fillInfo[0].lineage);
                            }
                            size_struct=1;
                        }
                        //if the structure is not empty
                        else if(size_struct>0){
                            //first we check if we already have the species and its informations
                            int check=0;
                            for(int i=0; i<size_struct; i++){
                                if(fillInfo[i].id==taxId){
                                     char string_id[MIN_SIZE];
                                    sprintf(string_id, "%d", fillInfo[i].id);

                                    strcat(contentHSP, "\"name\": \"");
                                    strcat(contentHSP, fillInfo[i].name);
                                    strcat(contentHSP, "\",\"taxid\":");
                                    strcat(contentHSP, string_id);
                                    strcat(contentHSP, ",\"rank\":\"");
                                    strcat(contentHSP, fillInfo[i].rank);
                                    strcat(contentHSP, "\",");
                                    if(strcmp(tabInfo[0][1], "1")==0){
                                        strcat(contentHSP, fillInfo[i].lineage);
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
                                        char string_id[MIN_SIZE];
                                        sprintf(string_id, "%d", taxId);

                                        strcat(contentHSP, "\"name\": \"");
                                        strcat(contentHSP, fillInfo[size_struct].name);
                                        strcat(contentHSP, "\",\"taxid\":");
                                        strcat(contentHSP, string_id);
                                        strcat(contentHSP, ",\"rank\":\"");
                                        strcat(contentHSP, fillInfo[size_struct].rank);
                                        strcat(contentHSP, "\",");
                                        if(strcmp(tabInfo[0][1], "1")==0){
                                            fillInfo[size_struct].lineage=createLineage(speciesInfo, species, hashmap); //ici on met la hashmap
                                            strcat(contentHSP, fillInfo[size_struct].lineage);
                                        }
                                        size_struct++;
                                        break;
                                    }
                                }
                                //if the species is not in the taxonomy.dat
                                else{
                                    //we display just the name of the species.
                                    strcat(contentHSP, "\"name\": \"");
                                    strcat(contentHSP, species);
                                    strcat(contentHSP, "\",\"taxid\": null,\"rank\": null,\"parent\": null");
                                    WARNING("Species : %s not found in taxonomy.dat", species);
                                }
                            }
                        }
                        strcat(contentHSP, "},");
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
                                strcat(contentHSP,getHSP(childNode,tabInfo[i][0]));
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
                            strcat(contentHSP, "\"query-cover\" : ");
                            char string_query_cover[MIN_SIZE];
                            sprintf(string_query_cover, "%d", querycover);
                            strcat(contentHSP, string_query_cover);
                            strcat(contentHSP, ",");
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
                                          strcat(contentHSP,getHSP(childNode,tabInfo[i][0]));
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
                            char string_query_cover[MIN_SIZE];
                            sprintf(string_query_cover, "%d", querycover);
                            strcat(contentHSP, string_query_cover);
                            strcat(contentHSP, ",");
                        }
                        
                    }
                    //we delete the last comma
                    int len_content = strlen(contentHSP);
                    contentHSP[len_content - 2] = '\0';
                    strcat(contentHSP,"},");
                }
            }   
        }
    }
    free(species);
    return strdup(contentHSP);


}

/**************************************************************************************************************/
/*                              getHSP: get the Info in the XML file                                          */
/** Parametre : node : Node where we are now                                                                  */
/**           : name :name of the element                                                                     */
/**           : label : name we put in the json file                                                          */
/**************************************************************************************************************/
char *getHSP(xmlNode *node, const char *name)
{ 
    char label[MIN_SIZE];
    strcpy(label, name+4);  
    if (strcmp(name, (const char *)node->name) == 0)
    {
        char content[MIN_SIZE];
        strcpy(content, "\"");
        strcat(content, label);
        strcat(content, "\" : ");
        strcat(content, (char *)xmlNodeGetContent(node));
        strcat(content, ",");
        return strdup(content);
    }
    return "";
}
