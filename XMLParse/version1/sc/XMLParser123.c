#include "methods.h"

void modechoice(int mode, xmlDoc *fichier, FILE *fp);
int main(int argc,char **argv){

/***************************************************************************************************/
/***********************************DÉCLARATION DES VARIABLES***************************************/
/***************************************************************************************************/
    FILE *fp = NULL;
    xmlDoc *fichier;
    char *filename;
/***************************************************************************************************/
/************************CONDITIONS VERIFIANT LE FICHIER XML A OUVRIR*******************************/
/***************************************************************************************************/
    if(argc!=2){
        printf("Oupsi... you need to add a XML file!\n");
        return 1;
    }
    printf("File : %s\n\n",argv[1]);
    
/***************************************************************************************************/
/************************************RECUP° DU FICHIER*********************************************/
/***************************************************************************************************/      
    filename= argv[1];
    char *compare;
    compare = strstr(filename,".xml");

/***************************************************************************************************/
/******************************VERIFIER SI C'EST UN FICHIER XML*************************************/
/***************************************************************************************************/   
    if(compare==NULL){
        printf("It's not a XML file!\n");
        return 1;
    }
    
/***************************************************************************************************/
/************************************LECTURE DU FICHIER*********************************************/
/***************************************************************************************************/   
    fichier=xmlReadFile(filename,NULL, 0);

    if(fichier==NULL){
        printf("The file is empty\n");
        return 1;
    }

/***************************************************************************************************/
/**************************************CHOIX DU MODE************************************************/
/***************************************************************************************************/   
    printf("Which mode do you want ? \n 1: Bronze mode\t\t2: Silver mode\t\t3: Gold mode\t\t4: I want to choose my own parameters\n");
    int mode; 
    scanf("%u",&mode);
    modechoice(mode, fichier, fp);

/***************************************************************************************************/
/****************************Si on veut rechoisir un autre mode ************************************/
/***************************************************************************************************/   
    while(1){
        printf("Do you want to choose another mode?\n Y or N ?\n");
        char answer[1];
        scanf("%s",answer);
        if(strcmp((const char*) answer, "Y")==0 || strcmp((const char*) answer, "y")==0 ){
            printf("Which mode do you want ? \n 1: Bronze mode\t\t2: Silver mode\t\t3: Gold mode\t\t4: I want to choose my own parameters\n");
            scanf("%u",&mode);
            modechoice(mode, fichier, fp);
        }
        else if (strcmp((const char*) answer, "N")==0 || strcmp((const char*) answer, "n")==0 ) {
            printf("Goodbye!\n");
            return 0;
        }
    }
    return 0;
}


void modechoice(int mode, xmlDoc *fichier, FILE *fp, char *name){
     switch(mode){
/***************************************************************************************************/
/******************************************BRONZE***************************************************/
/***************************************************************************************************/   
        case 1 :
            char *filename = strcat("../results/BronzeResults","_");
            filename = strcat(filename,name);
            filename=strcat(filename, ".csv");

            fp=fopen(filename, "w");
            blastOutPut_iteration(fichier, fp, 1, NULL);
            printf("You chose the Bronze mode\nYou can open the file : BronzeResult.csv\n");            
            break;
/***************************************************************************************************/
/******************************************ARGENT***************************************************/
/***************************************************************************************************/  
        case 2: 
            char *filename = strcat("../results/SilverResults","_");
            filename = strcat(filename,name);
            filename=strcat(filename, ".csv");
            fp=fopen(filename, "w");
            blastOutPut_iteration(fichier, fp, 2, NULL);
            printf("You chose the Silver mode\nYou can open the file : SilverResult.csv\n");
            break;
/***************************************************************************************************/
/******************************************GOLD*****************************************************/
/***************************************************************************************************/  
        case 3 :
        
        char *filename = strcat("../results/GoldResults","_");
            filename = strcat(filename,name);
            filename=strcat(filename, ".csv");
            fp=fopen(filename, "w");            blastOutPut_iteration(fichier, fp, 3, NULL);
            printf("You chose the Gold mode\nYou can open the file : GoldResult.csv\n");

            break;
/***************************************************************************************************/
/***********************************CHOIX DE L'UTILISATEUR******************************************/
/***************************************************************************************************/  
       default: break
     
    }
}

/* case 4: 
            fp =fopen("Results.csv", "r");
            printf("Please choose your parameters : \n"); 
           char *liste[]= {"hit id","bit-score","score","evalue","query-from","query-to","target-from","target-to","identity","positive","gaps","align-length"};
            char *newliste[2][12];
            
            for(int i=0; i<12; i++){
                newliste[0][i] = liste[i];
            }

            for(int i=0; i<12; i++){

                printf("%s : Y or N?\n",liste[i]);
                char answer[1];
                scanf("%s",answer);
                if(strcmp((const char *)answer, "Y")==0 || strcmp((const char *)answer, "y")==0){
                    newliste[1][i]="1";
                }
                else if(strcmp((const char *)answer, "N")==0 || strcmp((const char *)answer, "n")==0){
                    newliste[1][i]="0";
                }
                else{
                    printf("error\n");
                    exit(1);
                }
            }
           // test de remplissage du tableau 2D
            // for(int i =0; i<2; i++){
            //     for(int j =0; j<12; j++){
            //         printf("i = %u j = %u et valeur = %s\n", i,j,newliste[i][j]);
            //     }
            // }
            
            blastOutPut_iteration(fichier, fp, 4, newliste);

            break;
*/

 // /***************************************************************************************************/
            // /*************************************PARAMETRES A CHOISIR******************************************/
            // /***************************************************************************************************/
            