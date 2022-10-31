#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>
#define BUFSIZE 8192


char buf[BUFSIZE];
char content[BUFSIZE];
FILE *output;
int mode;

struct {
	char query[128];
	char access;
} state;



/** @brief Open the tag for the Bronze Mode
    @param data 
    @param name 
    @param attrs 
*/
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
	if(strcmp(name, "BlastOutput_query-def")==0){
		state.access=3;
	}
	if(strcmp(name, "BlastOutput_query-len")==0){
		state.access=4;
	}
}

/** @brief Open the tag for the Silver Mode
    @param data 
    @param name 
    @param attrs 
*/
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
	if(strcmp(name, "BlastOutput_query-def")==0){
		state.access=3;
	}
	if(strcmp(name, "BlastOutput_query-len")==0){
		state.access=4;
	}
}
/** @brief Open the tag for the Gold Mode
    @param data 
    @param name 
    @param attrs 
*/
void gold_tag_start(void *data, const char *name, const char **attrs) {

	if (strcmp(name, "Hit_accession") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_num") == 0){
		state.access = 1;
	}
	if (strcmp(name, "Hsp_bit-score") == 0){
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
	if(strcmp(name, "BlastOutput_query-def")==0){
		state.access=3;
	}
	if(strcmp(name, "BlastOutput_query-len")==0){
		state.access=4;
	}
	
}

/** @brief Closing the tag
    @param data 
    @param name 
*/
void tag_end(void *data, const char *name) {
}


/** @brief Read the tag content
    @param data 
    @param text 
    @param len 
*/
void tag_value(void *data, const char *text, int len) {
	strncpy(content, text, len);
	content[len] = '\0';
	
	if (state.access==1) {
		fprintf(output,"%s,", content);
		strcpy(content, state.query);
		state.access = 0;
	}
	if(state.access==2){
		fprintf(output,"%s\n", content);
		strcpy(content, state.query);
		state.access = 0;
	}
	if(state.access==3){
		fprintf(output, "Query-def :%s\n", content);
		strcpy(content, state.query);
		state.access = 0;
	}
	if(state.access==4){
		fprintf(output, "Length :%s\n\n", content);
		strcpy(content, state.query);
		if(mode ==1){
			fprintf(output, "id, num, score, evalue,align-length \n");
		}
		else if (mode ==2){
			fprintf(output, "id,hit,score,evalue,identity,positive,gaps,align-length \n");
		}
		else{
			fprintf(output, "id,hit,bitscore,score,evalue,query-from,query-to,target-from,target-to,identity,positive,gaps,align-length \n");

		}
		state.access = 0;
	}
}

/** @brief display errors and close file f
    @param f 
    @param parser 
*/
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
/**************************************************************************************/
/*********************************More than 2 args*************************************/
/**************************************************************************************/
	if (argc > 3) {
		printf("You need to put 0 or 2 arguments\n");
		printf("0 argument: The program will parse stdin.xml on the Gold mode\n");
		printf("2 arguments\targ1 : The program will parse either the number of XMLfiles or the XMLfile\n");
		printf("\t\targ2 : Mode choice: 1 (Bronze) | 2 (Silver) | 3 (Gold) | 4 (Personal)\n");
		exit(1);

	}
/**************************************************************************************/
/**************************************2 args******************************************/
/**************************************************************************************/
	else if(argc==3){
		FILE *f = fopen(argv[1], "r");
/**************************************************************************************/
/*************************************File empty***************************************/
/**************************************************************************************/
		if (!f) {
			fprintf(stderr, "Can't open blast file\n");
			exit(1);
		}
/**************************************************************************************/
/********************************2nd arg not an integer********************************/
/**************************************************************************************/
		if(atoi(argv[2])!=1 && atoi(argv[2])!= 2 && atoi(argv[2])!=3){
			fprintf(stderr, "arg2 : Mode choice: 1 (Bronze) | 2 (Silver) | 3 (Gold)\n");
			exit(1);
		}
		//substring the file name
		mode = atoi(argv[2]);
		char *name = argv[1];
        for (int i = 0; i < strlen(name); i++)
        {
            if (i >= strlen(name) - 4)
            {
                name[i] = '\0';
            }
            
        }
        if(strncmp (name,"../../",6) == 0){
            memmove(name, name+6, strlen(name));
        }
        else if (strncmp(name, "../", 3)==0){
            memmove(name, name+3, strlen(name));
        }
		else if(strncmp(name, "results/",8)==0){
			memmove(name, name+8, strlen(name));
		}

		char *filename= (char *)malloc(100);
		XML_Parser parser;

		switch(atoi(argv[2])){
/**************************************************************************************/
/***********************************Bronze mode****************************************/
/**************************************************************************************/
			case 1:
				strcpy(filename, "results/BronzeResults_");
				strcat(strcat(filename,argv[1]), ".csv");

				output = fopen(filename, "w");
				fprintf(output,"Bronze results of %s\n\n", name);
				
				parser = XML_ParserCreate(NULL);
				XML_SetElementHandler(parser, bronze_tag_start, tag_end);
				XML_SetCharacterDataHandler(parser, tag_value);

				state.access = 0;
				test_error(f, parser);
				break;
/**************************************************************************************/
/**********************************Silver mode*****************************************/
/**************************************************************************************/
			case 2 :
				strcpy(filename, "results/SilverResults_");
				strcat(strcat(filename,argv[1]), ".csv");
				
				output = fopen(filename, "w");
				fprintf(output,"Silver results of %s\n\n", name);
				
				parser = XML_ParserCreate(NULL);
				XML_SetElementHandler(parser, silver_tag_start, tag_end);
				XML_SetCharacterDataHandler(parser, tag_value);
				
				state.access = 0;
				test_error(f, parser);
		
				break;
/**************************************************************************************/
/************************************Gold Mode*****************************************/
/**************************************************************************************/
			case 3:
				strcpy(filename, "results/GoldResults_");
				strcat(strcat(filename,argv[1]), ".csv");

				output = fopen(filename, "w");
				fprintf(output,"Gold results of %s\n\n", name);
						
				parser = XML_ParserCreate(NULL);
				XML_SetElementHandler(parser, gold_tag_start, tag_end);
				XML_SetCharacterDataHandler(parser, tag_value);

				state.access = 0;
				test_error(f, parser);
		
				break;
			default : break;
		}
	}
/**************************************************************************************/
/************************************0 argument****************************************/
/**************************************************************************************/
	else if(argc ==1){
/**************************************************************************************/
/**************************We're gonna open the example file***************************/
/**************************************************************************************/
		FILE *f = fopen("stdin.xml", "r");
		if (!f) {
			fprintf(stderr, "Can't open blast file\n");
			exit(1);
		}
/**************************************************************************************/
/************************************Gold Mode*****************************************/
/**************************************************************************************/
		fprintf(stderr, "We're gonna display the Goldresults of stdin.xml\n");
		output = fopen("results/GoldResults_stdin.csv", "w");
		fprintf(output, "GoldResult of stdin.xml");
		fprintf(output, "id,hit,bitscore,score,evalue,query-from,query-to,target-from,target-to,identity,positive,gaps,align-length \n");

		XML_Parser parser = XML_ParserCreate(NULL);
		XML_SetElementHandler(parser, gold_tag_start, tag_end);
		XML_SetCharacterDataHandler(parser, tag_value);

		state.access = 0;

		test_error(f, parser);
	}
	return 0;
}