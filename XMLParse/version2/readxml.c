#include "readxml.h"

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

