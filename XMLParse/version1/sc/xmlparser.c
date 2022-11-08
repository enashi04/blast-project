#include "xmlparser.h"


void blastOutPut_iteration(xmlDoc *fichier, FILE *fp, int mode){
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
    selectedParameters(fp,mode);
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
                    hit_Iteration(child,fp, mode);
                }
            }           
        }
    }
}

//ENTRER DANS LE NOEUD : HIT
void hit_enter(xmlNode *node, FILE *fp){
    const char *name;
    name = "BlastOutput_query-len";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(fp, "Length : %s\n\n", xmlNodeGetContent(node));
    }
}
/**
 * RÉCUPÉRER LE NOM DE LA QUERY
*/
void query_Def(xmlNode *node, FILE *fp){

    const char *name;
    name = "BlastOutput_query-def";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(fp, "Query def : %s\n", xmlNodeGetContent(node));
    }
}

/**
 * RÉCUPÉRER LA LONGUEUR DE LA QUERY
*/
void query_Length(xmlNode *node, FILE *fp){
    const char *name;
    name = "BlastOutput_query-len";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(fp, "Length : %s\n\n", xmlNodeGetContent(node));
    }
} 
/**
 * identifier le hit id (hit id )
*/
char *hit_ID(xmlNode *node, FILE *fp, int mode){
    char *hit_id=NULL;
    xmlNode *child;
    const char *name="Hit_id"; 
    child = node ->children;
    for(node = child; node; node=node->next){
        if(strcmp(name, ( const char *)node->name)==0){
            hit_id =(char *)xmlNodeGetContent(node);
            strcat(hit_id,",");
        }
    }
    return hit_id;
}


/**
 * RÉCUPÉRER L'ÉLÉMENT VOULU
*/
void getHSP(xmlNode *node, const char *name, FILE *fp){
    if(strcmp(name, (const char *)node->name)==0){
/***************************************************************************************************/
/*************************SI ALIGN_LENGTH, ALORS ON ENLEVE LA VIRGULE*******************************/
/****************************************FIN DU CSV*************************************************/
/***************************************************************************************************/  
        if(strcmp(name, (const char *)"Hsp_align-len")==0){
            fprintf(fp,"%s",xmlNodeGetContent(node));
        }
        else{
            fprintf(fp,"%s,",xmlNodeGetContent(node));
        }
    }
}


/**
 * ENTRER DANS L'HSP
*/
void HSP_Enter(xmlNode *node, FILE *fp, int mode, char *hit_id){

    xmlNode *child;
    const char *name, *name2; 
/***************************************************************************************************/
/********************************CHILD = SOUS-NOEUD DU NODE ****************************************/
/***************************************************************************************************/   
    child = node ->children;
    name= "Hit_hsps";
    name2 = "Hsp";

    for(node = child; node; node=node->next){
        if(strcmp(name, ( const char *)node->name)==0){ 
/***************************************************************************************************/
/*****************************CHILDNODE = SOUS-NOEUD DE NODE***************************************/
/***************************************************************************************************/  
            xmlNode *childNode; 
            childNode = node->children;
            for(child = childNode; child; child=child->next){
/***************************************************************************************************/
/***************************NOEUD DANS LEQUEL ON SE TROUVE = HSP ?**********************************/
/***************************************************************************************************/   
                if(strcmp(name2, (const char *)child->name)==0){
                    fputs(hit_id,fp);
/***************************************************************************************************/
/*****************************LASTCHILD = SOUS-NOEUD DE CHILD***************************************/
/***************************************************************************************************/  
                    xmlNode *lastchild; 
                    lastchild=child->children;
                    for(childNode=lastchild; childNode; childNode=childNode->next){
/***************************************************************************************************/
/*****************************************MODE BRONZE***********************************************/
/***************************************************************************************************/  
                        if(mode ==1){
                            getHSP(childNode, "Hsp_num", fp);
                            getHSP(childNode, "Hsp_evalue",fp);
                            getHSP(childNode, "Hsp_score",fp);
                            getHSP(childNode, "Hsp_align-len", fp);

                        }
/***************************************************************************************************/
/****************************************MODE SILVER************************************************/
/***************************************************************************************************/  
                        else if (mode == 2){
                            getHSP(childNode, "Hsp_num", fp);
                            getHSP(childNode, "Hsp_identity", fp);
                            getHSP(childNode, "Hsp_evalue",fp);
                            getHSP(childNode, "Hsp_score",fp);
                            getHSP(childNode, "Hsp_positive",fp);
                            getHSP(childNode, "Hsp_gaps", fp);
                            getHSP(childNode, "Hsp_align-len", fp);
                        }
/***************************************************************************************************/
/*****************************************MODE GOLD*************************************************/
/***************************************************************************************************/  
                        else if (mode == 3){
                            getHSP(childNode, "Hsp_num", fp);
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
/***************************************************************************************************/
/****************************************MODE PERSO*************************************************/
/***************************************************************************************************/  
                        else{
                            printf("nothing\n");
                        }        
                    }   
                    fprintf(fp, "\n");  
                }
            }
        }
    }

}


/**
 * RÉCUPÉRATION DU HIT
*/
void hit_Iteration(xmlNode *node, FILE *fp, int mode){
    xmlNode *child;
    const char *iteration, *hit; 
/***************************************************************************************************/
/********************************CHILD = SOUS-NOEUD DU NODE ****************************************/
/***************************************************************************************************/   
    child = node->children;
    iteration = "Iteration_hits";
    hit= "Hit";
    int i=0; 
/***************************************************************************************************/
/***********************************PARCOURS DES SOUS-NOEUD*****************************************/
/***************************************************************************************************/   
    for(node = child; node; node =node->next){
        if(strcmp(iteration, (const char *)node->name)==0){
            xmlNode *childNode;
/***************************************************************************************************/
/********************************CHILDNODE = SOUS-NOEUD DE CHILD************************************/
/***************************************************************************************************/   
            childNode = node->children; 
            for(child=childNode; child; child=child->next){
                if(strcmp(hit, (const char *)child->name)==0){
                   i++;
/***************************************************************************************************/
/**********************************IDENTIFIER L'ID DU HIT*******************************************/
/***************************************************************************************************/   
                    
/***************************************************************************************************/
/*************************************ENTRER DANS L'HSP*********************************************/
/***************************************************************************************************/   
                    HSP_Enter(child, fp, mode, hit_ID(child,fp, mode));
                }
            }
        }
    }
}


void selectedParameters(FILE *fp,int mode){
/***************************************************************************************************/
/****************************************MODE BRONZE************************************************/
/***************************************************************************************************/ 
    if(mode==1){
        fprintf(fp, "hit id,hsp_num,score,evalue,align_length\n");
    }
/***************************************************************************************************/
/****************************************MODE SILVER************************************************/
/***************************************************************************************************/ 
    else if(mode == 2){
        fprintf(fp, "hit id,hsp_num,score,evalue,identity,positive,gaps,align_length\n");
    }
/***************************************************************************************************/
/*****************************************MODE GOLD*************************************************/
/***************************************************************************************************/ 
    else if(mode == 3){
        fprintf(fp, "hit id,hsp_num,bit-score,score,evalue,query-from,query-to,target-from,target-to,identity,positive,gaps,align-length \n");
    }
/***************************************************************************************************/
/****************************************MODE PERSO*************************************************/
/***************************************************************************************************/ 
    else{
        printf("papapa"); //ICI C'EST LE TEST
    }
}





