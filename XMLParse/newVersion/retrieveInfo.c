#include "retrieveInfo.h"
#define NUMCOLUMNS 3
#define NUMROWS 2147397 //nombre d'espèce

char name_hit[128];
char name_species[30];
static char tableau[NUMROWS][NUMCOLUMNS][128];
char taxoID[128], parentspecies[128], ranks[128], espece[128];

/**
 * RÉCUPÉRER LE NOM DE LA QUERY , début du formatage en json
*/
void query_Def(xmlNode *node){

    const char *name;
    name = "BlastOutput_query-def";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage  
        fprintf(output, "{\n\t\"query-name\" : \"%s\",\n", xmlNodeGetContent(node));
    }
}

/**
 * RÉCUPÉRER LA LONGUEUR DE LA QUERY
*/
void query_Length(xmlNode *node){
    const char *name;
    name = "BlastOutput_query-len";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(output, "\t\"query-length\" : \"%s\",\n\t\"hits\" :\n\t[\n", xmlNodeGetContent(node));
        //ici on met la query length pour le calcul de la query cover
        query_length = xmlNodeGetContent(node);
    }
} 

//ici on va parcourir le fichier pour récupérer les éléments que nous voulons avec libxml
void blastOutPut_iteration(xmlDoc *fichier, char *mode){
    //fprintf(stderr,"Nous sommes ici !\n");
    createTableau();
    xmlNode *node,*child, *root ;
/**********************************RECUP° DU PREMIER NOEUD******************************************/
    root = xmlDocGetRootElement(fichier);
/********************************CHILD = SOUS-NOEUD DU ROOT ****************************************/
    child = root->children;
    
    for(node=child; node; node=node->next){
        // fprintf(stderr,"les noeuds sont : %s\n", node->name);
        // fprintf(stderr, "mon parent est %s\n", node->parent->name);
        query_Def(node);
        query_Length(node);
    }
   // selectedParameters(mode);
    const char *nodeName, *iteration; 
    nodeName = "BlastOutput_iterations";
/**********************************PARCOURS DES SOUS-NOEUD******************************************/
    for(node=child; node; node=node->next){
/***************************SI ON EST DANS BlastOutput_iterations************************************/  
        if(strcmp(nodeName,(const char *)node->name)==0){
            xmlNode *childnode;
/********************************SOUS-NOEUD DE BLASTOUPUT....***************************************/  
            childnode = node->children;
            for(child=childnode; child; child=child->next){
/********************************ON ENTRE DANS ITERATION POUR***************************************/
                iteration = "Iteration";
                if(strcmp(iteration,(const char *)child->name)==0){
/********************************PARCOURS SOUS-NOEUD ITERATION**************************************/  
                    hit_Iteration(child, mode);
                }
            }           
        }
    }
}

/**
 * RÉCUPÉRATION DU HIT
*/
void hit_Iteration(xmlNode *node, char *mode){
    xmlNode *child;
    const char *iteration, *hit; 
/********************************CHILD = SOUS-NOEUD DU NODE ****************************************/   
    child = node->children;
    iteration = "Iteration_hits";
    hit= "Hit";
    int i=0; 
/***********************************PARCOURS DES SOUS-NOEUD*****************************************/   
    for(node = child; node; node =node->next){
        if(strcmp(iteration, (const char *)node->name)==0){
            xmlNode *childNode;
/********************************CHILDNODE = SOUS-NOEUD DE CHILD************************************/
            childNode = node->children; 
            for(child=childNode; child; child=child->next){
                if(strcmp(hit, (const char *)child->name)==0){
                   i++;
/**********************************IDENTIFIER L'ID DU HIT : ??????*******************************************/   
/*************************************ENTRER DANS L'HSP*********************************************/
                    HSP_Enter(child, mode, hit_ID(child, mode), getSpecies(child)); //ajouter un autre paramètres!
                }
            }
        }
    }
}

/**
 * identifier le hit id (hit id / hit_accession)
*/
char *hit_ID(xmlNode *node, char *mode){
    char *hit_id=NULL;
    xmlNode *child;
    const char *name="Hit_accession"; 
    child = node ->children;
    for(node = child; node; node=node->next){
        if(strcmp(name, ( const char *)node->name)==0){
            hit_id =(char *)xmlNodeGetContent(node);
           // strcat(hit_id," ");
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
    const char *name="Hit_def"; 
    child = node ->children;
    for(node = child; node; node=node->next){
        if(strcmp(name, ( const char *)node->name)==0){
            content =(char *)xmlNodeGetContent(node);
           // strcat(hit_id," ");
        }
    }
    //printf("content is %s --------", content);
    int debut=0, fin=0;
    for(int i =0; i<strlen(content); i++){
        if(content[i]=='['){ 
            debut = i;
        }
        if(content[i]==']'){
            fin =i;
            break;
        }
    }
    int j =0;
    for(int i = debut+1; i< fin ; i++){
        name_species[j]=content[i];
        j++;
    }
    name_species[j]='\0';
    //printf("species is : %s\n", name_species);
    return name_species;
}

/**
 * ENTRER DANS L'HSP
*/
void HSP_Enter(xmlNode *node, char *mode, char *hit_id, char *species){

    xmlNode *child;
    const char *name, *name2; 
/********************************CHILD = SOUS-NOEUD DU NODE ****************************************/   
    child = node ->children;
    name= "Hit_hsps";
    name2 = "Hsp";

    for(node = child; node; node=node->next){
        if(strcmp(name, ( const char *)node->name)==0){ 
/*****************************CHILDNODE = SOUS-NOEUD DE NODE***************************************/  
            xmlNode *childNode; 
            childNode = node->children;
            for(child = childNode; child; child=child->next){
/***************************NOEUD DANS LEQUEL ON SE TROUVE = HSP ?**********************************/
                if(strcmp(name2, (const char *)child->name)==0){
                    if(strcmp(name_hit, "")==0){
                        fprintf(output, "\t\t{\n\t\t\t\"hit_accession\" : \"%s\",\n", hit_id);
                        strcpy(name_hit, hit_id);
                    }
                    else{
                        fprintf(output, "\t\t,{\n\t\t\t\"hit_accession\" : \"%s\",\n", hit_id);
                    }

                    //mettre le lineage ici 
                    fprintf(output, "\t\t\t\"species\": [\n\t\t\t\t{\n"); 
                    //vérifie d'abord que la donnée est déjà stocké (pour ne pas reparcourir le tableau)
                    if(strcmp(species,espece)==0){
                        //name
                        fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n",species);
                         //taxid 1
                        fprintf(output, "\t\t\t\t\t\"taxid\": \"%s\",\n", taxoID); 
                        //parent 3
                        fprintf(output, "\t\t\t\t\t\"parent\": \"%s\",\n", parentspecies); 
                        //rang 2
                        fprintf(output, "\t\t\t\t\t\"rank\": \"%s\"\n", ranks); 
                        //fermeture
                        fprintf(output, "\t\t\t\t}\n\t\t\t],\n");

                    }
                   else  if(strcmp(species,espece)!=0){
                    int presence_species = 0;
                        for(int i=0; i<NUMROWS; i++){
                            if(strcmp(tableau[i][0], species)==0){
                                //name
                                presence_species=1;
                                fprintf(output, "\t\t\t\t\t\"name\":\"%s\",\n",species);
                                strcpy(espece, species);
                                //taxid 1
                                fprintf(output, "\t\t\t\t\t\"taxid\": \"%s\",\n", tableau[i][1]); 
                                strcpy(taxoID, tableau[i][1]);
                                //parent 3
                                fprintf(output, "\t\t\t\t\t\"parent\": \"%s\",\n", tableau[i][3]); 
                                strcpy(parentspecies, tableau[i][3]);
                                //rang 2
                                fprintf(output, "\t\t\t\t\t\"rank\": \"%s\"\n", tableau[i][2]); 
                                strcpy(ranks, tableau[i][2]);
                                //fermeture
                                fprintf(output, "\t\t\t\t}\n\t\t\t],\n");
                                break;
                            }
                        }
                        if(presence_species==0){
                            fprintf(output, "\t\t\t\t\t\"name\":\"%s\"\n",species);
                            fprintf(output, "\t\t\t\t}\n\t\t\t],\n");
                        }
                   }
                  
/*****************************LASTCHILD = SOUS-NOEUD DE CHILD***************************************/
                    xmlNode *lastchild; 
                    lastchild=child->children;
                    for(childNode=lastchild; childNode; childNode=childNode->next){
/*****************************************MODE BRONZE***********************************************/
                        if(strcmp(mode, "bronze") == 0){
                            getHSP(childNode, "Hsp_num", "number of hit");
                            getHSP(childNode, "Hsp_identity", "identity");
                            getHSP(childNode, "Hsp_align-len", "align_len");
                            getHSP(childNode, "Hsp_gaps","gaps");
                            getHSP(childNode, "Hsp_query-from", "query_from");
                            getHSP(childNode, "Hsp_query-to", "query_to");
                            getHSP(childNode, "Hsp_hit-from", "target_from");
                            getHSP(childNode, "Hsp_hit-to", "target_to");
                            getHSP(childNode, "Hsp_positive", "positive");
                            getHSP(childNode, "Hsp_evalue", "evalue");
                            getHSP(childNode, "Hsp_score","score");
                            getHSP(childNode, "Hsp_bit-score", "bitscore");

                        }
/****************************************MODE SILVER************************************************/
                        else if (strcmp(mode, "silver") == 0){
                            getHSP(childNode, "Hsp_num", "number of hit");
                            getHSP(childNode, "Hsp_identity", "identity");
                            getHSP(childNode, "Hsp_align-len", "align_len");
                            getHSP(childNode, "Hsp_gaps","gaps");
                            getHSP(childNode, "Hsp_query-from", "query_from");
                            getHSP(childNode, "Hsp_query-to", "query_to");
                            getHSP(childNode, "Hsp_hit-from", "target_from");
                            getHSP(childNode, "Hsp_hit-to", "target_to");
                            getHSP(childNode, "Hsp_positive", "positive");
                            getHSP(childNode, "Hsp_evalue", "evalue");
                            getHSP(childNode, "Hsp_score","score");
                            getHSP(childNode, "Hsp_bit-score", "bitscore");
                        }
/*****************************************MODE GOLD*************************************************/
                        else if (strcmp(mode, "gold") == 0){
                            getHSP(childNode, "Hsp_num", "number of hit");
                            getHSP(childNode, "Hsp_identity", "identity");
                            getHSP(childNode, "Hsp_align-len", "align_len");
                            getHSP(childNode, "Hsp_gaps","gaps");
                            getHSP(childNode, "Hsp_query-from", "query_from");
                            getHSP(childNode, "Hsp_query-to", "query_to");
                            getHSP(childNode, "Hsp_hit-from", "target_from");
                            getHSP(childNode, "Hsp_hit-to", "target_to");
                            getHSP(childNode, "Hsp_positive", "positive");
                            getHSP(childNode, "Hsp_evalue", "evalue");
                            getHSP(childNode, "Hsp_score","score");
                            getHSP(childNode, "Hsp_bit-score", "bitscore");
                        }
/****************************************MODE PERSO*************************************************/  
                        else{
                            printf("nothing\n");
                        }        
                    }   
                    fprintf(output, "\n");  
                }
            }
        }
    }

}

/**
 * RÉCUPÉRER L'ÉLÉMENT VOULU
*/
void getHSP(xmlNode *node, const char *name, char *label){ //ajout d'un autre char *
    if(strcmp(name, (const char *)node->name)==0){
/******************************Ajout de la query cover **************************************/
        if(strcmp(name, (const char *)"Hsp_align-len")==0){
            //on met la query cover ici ! 
            int querycover = 100 * (t_to - t_from) / query_length;
            fprintf(output, "\t\t\t\"query-cover\" : \"%d\",\n", querycover);
            fprintf(output,"\t\t\t\"%s\" : \"%s\"\n\t\t}",label, xmlNodeGetContent(node));
        }
/********************************récupération du tfrom****************************/
        else if(strcmp(name,(const char *)"Hsp_hit_from")==0){
            t_from = xmlNodeGetContent(node); 
            fprintf(output,"\t\t\t\"%s\" : \"%s\",\n",label, xmlNodeGetContent(node));
        }
/********************************récupération du to****************************/
        else if(strcmp(name,(const char *)"Hsp_hit_to")==0){
            t_to= xmlNodeGetContent(node);
            fprintf(output,"\t\t\t\"%s\" : \"%s\",\n",label, xmlNodeGetContent(node));
        }
        else{
            fprintf(output,"\t\t\t\"%s\" : \"%s\",\n",label, xmlNodeGetContent(node));
        }
    }
}

/***********Remplir un tableau contenant les taxid et compagnie*************/
char (*createTableau())[3][128] {
    FILE *f = fopen("taxonomy.reduced", "r");
    char taxID[128], name[128], rank[128], parentID[128];
    char l[BUFSIZE];
    if (fgets(l, BUFSIZE, f) == NULL){
        fprintf(stderr,"Empty taxo file");
    }
    int i =1;
    //on va d'abord remplir la première ligne !
    sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank,parentID);
   
    strcpy(tableau[0][0], name);
    strcpy(tableau[0][1], taxID);
    strcpy(tableau[0][2], rank);
    strcpy(tableau[0][3], parentID);

   while(fgets(l, BUFSIZE, f) != NULL){
    sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank,parentID);
        for(int j = 0; j < NUMCOLUMNS; j++){
            if(j==0){
                strcpy(tableau[i][j],name); //species
            }
            else if(j==1){
                strcpy(tableau[i][j],taxID);
            }
            else if(j==2){
                strcpy(tableau[i][j],rank); 
            }
            else if(j==3){
                strcpy(tableau[i][j],parentID);
            }
        }
        i++;
    }
   return tableau;
} 