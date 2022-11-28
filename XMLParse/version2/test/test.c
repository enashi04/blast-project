#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv){
    
    return 0;
}

//récupération du nom de l'espèce
void speciesName(char *species){
    char *converted = "HAMP domain-containing histidine kinase [Mycobacterium tuberculosis] &gt;emb|CMI28831.1| two component sensor histidine kinase [Mycobacterium tuberculosis] &gt;emb|CMM73239.1| two component sensor histidine kinase [Mycobacterium tuberculosis]";//sera en param de la fonction 
    char *translated = (char *)malloc(strlen(converted));
    for(int i =0; i<strlen(converted); i++){
        if(converted[i]=='['){ 
            printf("%u : %c\n",i,converted[i]);
            memmove(translated,converted +i, strlen(converted));
            break;
        }
    }
    char *newword = (char *)malloc(strlen(translated));
    int j =0;
    for(int i = 0; i<strlen(translated); i++){
        if(translated[i]!='[' && translated[i]!=']'){
            newword[j] = translated[i];
            j++;
        }
        else if(translated[i]==']'){
            break;
        }
    }
    printf("%s : is the word \n", newword);
}