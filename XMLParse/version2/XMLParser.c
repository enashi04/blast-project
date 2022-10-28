#include "readxml.h"
#include "argstype.h"

int main(int argc, char *argv[]) {
	if (argc > 3) {
        return moreThanTwoArgs();
	}
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
