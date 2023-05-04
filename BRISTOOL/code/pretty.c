#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//fonction prettier
void prettier(char *json) {
    int i, level = 0;
    int len = strlen(json);

    for (i = 0; i < len; i++) {
        char c = json[i];

        switch (c) {
            case '{':
            case '[':
                level++;
                printf("%c\n", c);
                 for (int j = 0; j < level; j++) {
                    printf("  ");
                }
                break;

            case '}':
            case ']':
                level--;
                printf("\n");
                for (int j = 0; j < level; j++) {
                    printf("  ");
                }
                printf("%c", c);
                break;

            case ',':
                printf(",\n");
                for (int j = 0; j < level; j++) {
                    printf("  ");
                }
                break;

            default:
                printf("%c", c);
                break;
        }
    }
}

int main (int argc, char **argv){
    char *json = "{\"blast-version\" : \"BLASTP 2.13.0+\",\"db\" : \"nr\",\"blast-output\":[{\"query-name\" : \"sp|P96368|TRCS_MYCTU Sensor histidine kinase TrcS OS=Mycobacterium tuberculosis (strain ATCC 25618 / H37Rv) OX=83332 GN=trcS PE=1 SV=1\",\"query-length\" : 509,\"species\": {\"taxid\" : 83332,\"name\" : \"Mycobacterium tuberculosis (strain ATCC 25618 / H37Rv)\"}}]}";
    prettier(json);
} 