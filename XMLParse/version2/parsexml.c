#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>
#define BUFSIZE 8192


char buf[BUFSIZE];
char content[BUFSIZE];

struct {
	char query[128];
	char access;
} state;

void bronze_tag_start(void *data, const char *name, const char **attrs){
	if (strcmp(name, "Hit_accession") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_num") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_score") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_evalue") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_align-len") == 0){
		state.access = 2;
	}
}

void silver_tag_start(void *data, const char *name, const char **attrs){
	if (strcmp(name, "Hit_accession") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_num") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_score") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_evalue") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_identity") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_positive") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_gaps") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_align-len") == 0){
		state.access = 2;
	}
}
// Opening tag
void gold_tag_start(void *data, const char *name, const char **attrs) {
	if (strcmp(name, "Hit_accession") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_num") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_bitscore") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_score") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_evalue") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_query-from") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_query-to") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_hit-from") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_hit-to") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_identity") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_positive") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_gaps") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_align-len") == 0){
		state.access = 2;
	}
	
}

// Closing tag
void tag_end(void *data, const char *name) {
}

// Reading tag content
void tag_value(void *data, const char *text, int len) {
	strncpy(content, text, len);
	content[len] = '\0';
	
	if (state.access==1) {
		printf("%s,", content);
		strcpy(content, state.query);
		state.access = 0;
	}
	if(state.access==2){
		printf("%s\n", content);
		strcpy(content, state.query);
		state.access = 0;
	}
}

void test_error(FILE *f, XML_Parser parser){
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

int main(int argc, char *argv[]) {
	if (argc > 3) {
		printf("You need to put 0 or 2 arguments\n");
		printf("0 argument: The program will parse stdin.xml on the Gold mode\n");
		printf("2 arguments\targ1 : The program will parse either the number of XMLfiles or the XMLfile\n");
		printf("\t\targ2 : Mode choice: 1 (Bronze) | 2 (Silver) | 3 (Gold) | 4 (Personal)\n");
		exit(1);

	}
	else if(argc==3){
		FILE *f = fopen(argv[1], "r");
		if (!f) {
			fprintf(stderr, "Can't open blast file\n");
			exit(1);
		}
		if(atoi(argv[2])!=1 && atoi(argv[2])!= 2 && atoi(argv[2])!=3){
			fprintf(stderr, "arg2 : Mode choice: 1 (Bronze) | 2 (Silver) | 3 (Gold)\n");
			exit(1);
		}
		printf("On commence le parsing du document\n");
		char *filename= (char *)malloc(100);
		XML_Parser parser ;
		switch(atoi(argv[2])){
			case 1:
				printf("on passe ici?");
				strcpy(filename, "../BronzeResults_");
				strcat(strcat(filename,argv[1]), ".csv");
				parser = XML_ParserCreate(NULL);
				XML_SetElementHandler(parser, bronze_tag_start, tag_end);
				XML_SetCharacterDataHandler(parser, tag_value);

				state.access = 0;
				test_error(f, parser);
		
				break;
			case 2 :
				strcpy(filename, "../SilverResults_");
				strcat(strcat(filename,argv[1]), ".csv");
				parser = XML_ParserCreate(NULL);
				XML_SetElementHandler(parser, silver_tag_start, tag_end);
				XML_SetCharacterDataHandler(parser, tag_value);
				
				state.access = 0;
				test_error(f, parser);
		
				break;
			case 3:
				strcpy(filename, "../GoldResults_");
				strcat(strcat(filename,argv[1]), ".csv");
				parser = XML_ParserCreate(NULL);
				XML_SetElementHandler(parser, gold_tag_start, tag_end);
				XML_SetCharacterDataHandler(parser, tag_value);

				state.access = 0;
				test_error(f, parser);
		
				break;
			default : break;
		}
		
	}
	else if(argc ==1){
		FILE *f = fopen("stdin.xml", "r");
		if (!f) {
			fprintf(stderr, "Can't open blast file\n");
			exit(1);
		}
		XML_Parser parser = XML_ParserCreate(NULL);
		XML_SetElementHandler(parser, gold_tag_start, tag_end);
		XML_SetCharacterDataHandler(parser, tag_value);

		state.access = 0;

		test_error(f, parser);
	}
	return 0;
}