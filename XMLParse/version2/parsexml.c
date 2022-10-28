#include <stdio.h>
#include <string.h>
#include <expat.h>
#define BUFSIZE 8192


char buf[BUFSIZE];
char content[BUFSIZE];

struct {
	char query[128];
	char access;
} state;

// Opening tag
void tag_start(void *data, const char *name, const char **attrs) {
	if (strcmp(name, "Hit_accession") == 0)
		state.access = 1;
}

// Closing tag
void tag_end(void *data, const char *name) {
}

// Reading tag content
void tag_value(void *data, const char *text, int len) {
	strncpy(content, text, len);
	content[len] = '\0';
	
	if (state.access) {
		printf("accession id: %s\n", content);
		strcpy(content, state.query);
		state.access = 0;
	}
}

int main(int argc, char *argv[]) {
	if (argc > 3) {
		printf("You need to put 0 or 2 arguments\n");
		printf("0 argument: The program will parse stdin.xml on three differents modes\n");
		printf("2 arguments\targ1 : The program will parse either the number of XMLfiles or the XMLfile\n");
		printf("\t\targ2 : Mode choice: 1 (Bronze) | 2 (Silver) | 3 (Gold) | 4 (Personal)\n");
		exit(1);

	}
	else if(argc==2){
		FILE *f = fopen(argv[1], "r");
		if (!f) {
			fprintf(stderr, "Can't open blast file\n");
			exit(1);
		}
		XML_Parser parser = XML_ParserCreate(NULL);
		XML_SetElementHandler(parser, tag_start, tag_end);
		XML_SetCharacterDataHandler(parser, tag_value);

		state.access = 0;

		int done;
		do {
			int len = fread(buf, 1, BUFSIZE, f);

			if (ferror(f)) {
				fprintf(stderr, "Read error\n");
				break;
			}
			done = feof(f);

			if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
				fprintf(stderr, "Error while parsing XML\n");
				break;
			}
		} while (!done);

		XML_ParserFree(parser);
		fclose(f);
	}
	else if(argc ==1){
		FILE *f = fopen("stdin.xml", "r");
		if (!f) {
			fprintf(stderr, "Can't open blast file\n");
			exit(1);
		}
		XML_Parser parser = XML_ParserCreate(NULL);
		XML_SetElementHandler(parser, tag_start, tag_end);
		XML_SetCharacterDataHandler(parser, tag_value);

		state.access = 0;

		int done;
		do {
			int len = fread(buf, 1, BUFSIZE, f);

			if (ferror(f)) {
				fprintf(stderr, "Read error\n");
				break;
			}
			done = feof(f);

			if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
				fprintf(stderr, "Error while parsing XML\n");
				break;
			}
		} while (!done);

		XML_ParserFree(parser);
		fclose(f);
	}
	return 0;
}