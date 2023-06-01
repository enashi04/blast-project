#include "pretty.h"

//ajout d'un argument ici (le i à incrémenter)
void prettier(char *json, int iteration) {
    int level = iteration;
    int len = strlen(json);

    for (int i = 0; i < len; i++) {
        char c = json[i];
        switch (c) {
            case '{': 
               if((isalpha(json[i-1]) || isdigit(json[i-1])) && (isalpha(json[i+1]) || isdigit(json[i+1]))){
                    fprintf(output, "%c", c);
                    break;
               }
            case '[':
                level++;
                fprintf(output,"%c\n", c);
                 for (int j = 0; j < level; j++) {
                    fprintf(output,"  ");
                }
                break;

            case '}':
                if(json[i+1]=='\"'){
                    fprintf(output, "%c",c);
                    break;
                }
                else if((isalpha(json[i-1]) || isdigit(json[i-1])) && (isalpha(json[i+1]) || isdigit(json[i+1])) ){
                    fprintf(output, "%c", c);
                    break;
            }
               
            case ']':
                level--;
                fprintf(output,"\n");
                for (int j = 0; j < level; j++) {
                    fprintf(output,"  ");
                }
                fprintf(output,"%c", c);
                break;

            case ',':
                fprintf(output,",\n");
                for (int j = 0; j < level; j++) {
                    fprintf(output,"  ");
                }
                break;

            default:
                fprintf(output,"%c", c);
                break;
        }
    }
}

