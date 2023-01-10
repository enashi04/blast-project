


char *replaceWord(const char *s, const char *oldW, const char *newW);

void blast_version(xmlNode *node, FILE *output);

void blast_reference(xmlNode *node, FILE *output);

void blast_db(xmlNode *node, FILE *output);

void query_Def(xmlNode *node, FILE *output);

void query_Length(xmlNode *node, FILE *output);

void hitNode(xmlNode *node, FILE *output);

void hspNode(xmlNode *node, FILE *output);

void iterationNode(xmlNode *node, FILE *output);

void blastInfo(xmlDoc *xmlfile, FILE *output, xmlNode *child);