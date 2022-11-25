#include <stdio.h>
#include <stdlib.h>
#include "mempool.h"
#include "vector.h"

#define uint unsigned int
#define str char*

#define LOG(lvl, ...) { fprintf(stderr, "%s: ", lvl); \
						fprintf(stderr, __VA_ARGS__); \
						fprintf(stderr, "\n"); }
#define FATAL(...) { LOG("FATAL", __VA_ARGS__); \
					exit(EXIT_FAILURE); }
#define ERROR( ...) LOG("ERROR", __VA_ARGS__);
#define WARNING( ...) LOG("WARNING", __VA_ARGS__);
#define INFO(...) LOG("INFO", __VA_ARGS__);
#define DEBUG(...) LOG("DEBUG", __VA_ARGS__);

#define BUFSIZE 1024
#define ESTSPECIES 2429415

DECLARE_VECTOR(str)
DEFINE_VECTOR(str)

vector_str *taxons;
mempool *pool;

static void store_taxon(uint taxid, const char *name) {
	char *stored_name = mempool_store(pool, name);
	vector_str_set(taxons, taxid, stored_name);
}

static char* get_taxon(uint taxid) {
	return taxons->data[taxid];
}

int main() {
	/*
		Parse file created with:
		zcat /commun/banbi/taxonomy/production/taxonomy.dat | tail -n+2 | grep -v $'\tno rank\t' | cut -d$'\t' -f1,3 > taxonomy.reduced
	*/
	char l[BUFSIZE];
	char tmp[512];
	uint taxid;
	FILE *f;

	taxons = vector_str_init(ESTSPECIES);
	pool = mempool_init(ESTSPECIES*32);
	
	if ((f = fopen("taxonomy.reduced", "r")) == NULL)
		FATAL("Can't open taxo file");
	if (fgets(l, BUFSIZE, f) == NULL)
		FATAL("Empty taxo file");

	while (fgets(l, BUFSIZE, f) != NULL) {
		sscanf(l, "%u	%[^\n]", &taxid, tmp);
		store_taxon(taxid, tmp);
	}
	fclose(f);

	uint test_taxids[] = { 9606, 4932, 9783 };
	for (uint i=0; i<3; i++)
		printf("%u : %s\n", test_taxids[i], get_taxon(test_taxids[i]));

	mempool_free(pool);
	vector_str_free(taxons);
}
