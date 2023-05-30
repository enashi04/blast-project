#include "pretty.h"

//ajout d'un argument ici (le i à incrémenter)
void prettier(char *json) {
    int i, level = 0;
    int len = strlen(json);

    for (i = 0; i < len; i++) {
        char c = json[i];

        switch (c) {
            case '{':
            case '[':
                level++;
                fprintf(output,"%c\n", c);
                 for (int j = 0; j < level; j++) {
                    fprintf(output,"  ");
                }
                break;

            case '}':
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