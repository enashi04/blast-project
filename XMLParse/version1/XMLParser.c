#include "methods.h"

void modechoice(int mode, xmlDoc *fichier, FILE *fp){
     switch(mode){
/***************************************************************************************************/
/******************************************BRONZE***************************************************/
/***************************************************************************************************/   
        case 1 :
            fp=fopen("BronzeResult.csv", "w");
            blastOutPut_iteration(fichier, fp, 1, NULL);
            printf("You chose the Bronze mode\nYou can open the file : BronzeResult.csv\n");            
            break;
/***************************************************************************************************/
/******************************************ARGENT***************************************************/
/***************************************************************************************************/  
        case 2: 
            fp=fopen("SilverResult.csv", "w");
            blastOutPut_iteration(fichier, fp, 2, NULL);
            printf("You chose the Silver mode\nYou can open the file : SilverResult.csv\n");
            break;
/***************************************************************************************************/
/******************************************GOLD*****************************************************/
/***************************************************************************************************/  
        case 3 :
            fp=fopen("GoldResult.csv", "w");
            blastOutPut_iteration(fichier, fp, 3, NULL);
            printf("You chose the Gold mode\nYou can open the file : GoldResult.csv\n");

            break;
/***************************************************************************************************/
/***********************************CHOIX DE L'UTILISATEUR******************************************/
/***************************************************************************************************/  
        case 4: 
            fp =fopen("Results.csv", "r");
            printf("Please choose your parameters : \n"); 

            //SOUS FORME DE LISTE POUR AJOUTER LES ARGUMENTS
            char *liste[]= {"hit id","bit-score","score","evalue","query-from","query-to","target-from","target-to","identity","positive","gaps","align-length"};

            printf("la taille du tableau est %lu\n", sizeof(liste));
            int j =0;
            for(int i =0; i<12; i++){
                printf("%s : Y or N?\n",liste[i]);
                char answer[1];
                scanf("%s",answer);
                if(strcmp((const char*) answer, "N") ==0 || strcmp((const char*) answer, "n")==0 ){
                    j++;
                    liste[i]=NULL;
                    printf("newliste is %s\n", liste[i]);
                }
            }
            int length = 12-j;
            printf("la longueur de length est %u\n", length);

            char *newliste[length];

            int i=0;
            for(int k =0; k<12; k++){
                printf("on entre ici ? ");
                if(liste[k]!=NULL){
                    printf("on entre ici ? ");
                    newliste[i]=liste[k];
                    i++;
                }
            }
            
            //vérification
            for(int i =0; i<length; i++){
                printf("%s | ", newliste[i]);
            }
            blastOutPut_iteration(fichier, fp, 4, liste);
            break;

        
     
    }
}

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
    //fonction qui fait appel au choix du mode.
    modechoice(mode, fichier, fp);

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

