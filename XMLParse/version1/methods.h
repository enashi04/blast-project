#include "libxml/parser.h"
#include "libxml/tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//entrer dans le noeud hit
void hit_enter(xmlNode *node, FILE *fp){
    const char *name;
    name = "BlastOutput_query-len";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(fp, "Length : %s\n\n", xmlNodeGetContent(node));
    }
}

void query_Def(xmlNode *node, FILE *fp){

    const char *name;
    name = "BlastOutput_query-def";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(fp, "Query def : %s\n", xmlNodeGetContent(node));
    }
}

//query length
void query_Length(xmlNode *node, FILE *fp){
    const char *name;
    name = "BlastOutput_query-len";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(fp, "Length : %s\n\n", xmlNodeGetContent(node));
    }
} 
//identifier le hit id (hit id or hit def???)
void hit_ID(xmlNode *node, FILE *fp, int mode, char *liste[]){
    xmlNode *child;
    const char *name; 
    child = node ->children;
    name= "Hit_id";
    for(node = child; node; node=node->next){
        if(strcmp(name, ( const char *)node->name)==0){
            if(mode==1){
                fprintf(fp, "hit id, score, evalue, align_length\n");
            }
            else if(mode == 2){
                fprintf(fp, "hit id, score, evalue, identity, positive, gaps, align_length\n");
            }
            else if(mode == 3){
                fprintf(fp, "hit id ,bit-score,score,evalue,query-from,query-to,target-from,target-to,identity,positive,gaps,align-length \n");
            }
            else{

            }
           // fprintf(fp, "hit id ,bit-score,score,evalue,query-from,query-to,target-from,target-to,identity,positive,gaps,align-length \n");
            fprintf(fp, "%s,", xmlNodeGetContent(node));
        }
    }
}


//identifier l'identité
void getHSP(xmlNode *node, const char *name, FILE *fp){
    if(strcmp(name, (const char *)node->name)==0){
        if(strcmp(name, (const char *)"Hsp_align-len")==0){
            fprintf(fp," %s",xmlNodeGetContent(node));
        }
        else{
            fprintf(fp," %s,",xmlNodeGetContent(node));
        }
    }
}


//rentrer dans hsp
void HSP_Enter(xmlNode *node, FILE *fp, int mode, char *liste[]){
    xmlNode *child;
    const char *name, *name2; 
    child = node ->children;
    name= "Hit_hsps";
    name2 = "Hsp";
    //printf("test \n");

    for(node = child; node; node=node->next){
        if(strcmp(name, ( const char *)node->name)==0){ 
            xmlNode *childNode;
            childNode = node->children;
           
            for(child = childNode; child; child=child->next){

                if(strcmp(name2, (const char *)child->name)==0){
                    //dernière boucle et on mets les fonctions à l'intérieur pour récupérer les éléments qu'on veut !
                    xmlNode *lastchild; 
                    lastchild=child->children;

                    for(childNode=lastchild; childNode; childNode=childNode->next){
                        if(mode ==1){
                            getHSP(childNode, "Hsp_evalue",fp);
                            getHSP(childNode, "Hsp_score",fp);
                            getHSP(childNode, "Hsp_align-len", fp);

                        }
                        else if (mode == 2){
                            getHSP(childNode, "Hsp_identity", fp);
                            getHSP(childNode, "Hsp_evalue",fp);
                            getHSP(childNode, "Hsp_score",fp);
                            getHSP(childNode, "Hsp_positive",fp);
                            getHSP(childNode, "Hsp_gaps", fp);
                            getHSP(childNode, "Hsp_align-len", fp);
                        }
                        else if (mode == 3){
                            //les méthodes pour chercher les infos qu'on veut 
                            getHSP(childNode, "Hsp_identity", fp);
                            getHSP(childNode, "Hsp_align-len", fp);
                            getHSP(childNode, "Hsp_gaps", fp);
                            getHSP(childNode, "Hsp_query-from",fp);
                            getHSP(childNode, "Hsp_query-to",fp);
                            getHSP(childNode, "Hsp_hit-from",fp);
                            getHSP(childNode, "Hsp_hit-to",fp);
                            getHSP(childNode, "Hsp_positive",fp);
                            getHSP(childNode, "Hsp_evalue",fp);
                            getHSP(childNode, "Hsp_score",fp);
                            getHSP(childNode, "Hsp_bit-score",fp);
                        }
                        else{
                            printf("nothing\n");
                        }   
                            
                    }
                       
                    
                }
            }
        }
    }
    fprintf(fp, "\n \n");
}


//récupérer le hit
void hit_Iteration(xmlNode *node, FILE *fp, int mode, char *liste[]){
    xmlNode *child;
    const char *iteration, *hit; 
    child = node->children;
    iteration = "Iteration_hits";
    hit= "Hit";
    int i=0; 
    for(node = child; node; node =node->next){
        if(strcmp(iteration, (const char *)node->name)==0){
            xmlNode *childNode;
            childNode = node->children; 
            for(child=childNode; child; child=child->next){
                if(strcmp(hit, (const char *)child->name)==0){
                   i++;
                    //on met les fonctions qui va permettre de récupérer les données qu'on veut
                   // fprintf(fp, "%u.\n", i);
                    hit_ID(child,fp, mode, liste);
                    //mettre condition ici pour savoir quoi prendre.
                    HSP_Enter(child, fp, mode, liste);
                    //break;
                }
            }
        }
    }
}

void blastOutPut_iteration(xmlDoc *fichier, FILE *fp, int mode, char *liste[]){

    xmlNode *node,*child, *root ;

/***************************************************************************************************/
/**********************************RECUP° DU PREMIER NOEUD******************************************/
/***************************************************************************************************/   
    root = xmlDocGetRootElement(fichier);

/***************************************************************************************************/
/********************************CHILD = SOUS-NOEUD DU ROOT ****************************************/
/***************************************************************************************************/   
    child = root->children;
    
    for(node=child; node; node=node->next){
        query_Def(node, fp);
        query_Length(node,fp);
    }

    const char *nodeName, *iteration; 
    nodeName = "BlastOutput_iterations";
/***************************************************************************************************/
/**********************************PARCOURS DES SOUS-NOEUD******************************************/
/***************************************************************************************************/  
    for(node=child; node; node=node->next){
/***************************************************************************************************/
/***************************SI ON EST DANS BlastOutput_iterations************************************/
/***************************************************************************************************/  
        if(strcmp(nodeName,(const char *)node->name)==0){
            xmlNode *childnode;
/***************************************************************************************************/
/********************************SOUS-NOEUD DE BLASTOUPUT....***************************************/
/***************************************************************************************************/  
            childnode = node->children;
            for(child=childnode; child; child=child->next){
/***************************************************************************************************/
/********************************ON ENTRE DANS ITERATION POUR***************************************/
/***************************************************************************************************/  
                iteration = "Iteration";
                if(strcmp(iteration,(const char *)child->name)==0){
/***************************************************************************************************/
/********************************PARCOURS SOUS-NOEUD ITERATION**************************************/
/***************************************************************************************************/  
                    hit_Iteration(child,fp, mode, liste);
                }
            }           
        }
    }
}






