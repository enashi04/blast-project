#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE 1024
#define CAPACITY 2429415 //nbre de ligne du fichier taxo


//on définit les items
typedef struct Ht_item Ht_item;
struct Ht_item {
    char* key;
    char* value;
};

//on définit la hashtable
typedef struct HashTable HashTable;
struct HashTable {
    // Contains an array of pointers to items
    Ht_item** items;
    int size;
    int count;
};

int hash_index(HashTable *table, char *key){
    int index = -1;
    for(int i = 0; i< table->size; i++){
        if(strcmp(table->items[i]->key, key)==0){
            index= i ;
            break;
        }
    }
    return index;
}

//création d'un item
Ht_item* create_item(char* key, char* value) {
    // Creates a pointer to a new hash table item
    Ht_item* item = (Ht_item*) malloc (sizeof(Ht_item));
    item->key = (char*) malloc(strlen(key) + 1);//malloc (strlen(key) + 1)
    item->value = (char*) malloc (strlen(value) +1);
    
    strcpy(item->key, key); 
    strcpy(item->value, value);

    return item; //structure contenant clé + value
}

//création d'une hashtable
HashTable* create_table(int size) {
   
    HashTable* table = (HashTable*) malloc (sizeof(HashTable));
    table->size = size; //nbre de ligne dans le fichier taxo
    table->count = 0;
    table->items = (Ht_item**) calloc (table->size, sizeof(Ht_item*));
    for (int i=0; i<table->size; i++)
        table->items[i] = NULL; //table vide au départ
    return table;
}

// Frees an item
void free_item(Ht_item* item) {
    free(item->key);
    free(item->value);
    free(item);
}

//frees the table
void free_table(HashTable* table) {
    for (int i=0; i<table->size; i++) {
        Ht_item* item = table->items[i];
        if (item != NULL)
            free_item(item);
    }

    free(table->items);
    free(table);
}

//insérer l'item dans la hashmap
void ht_insert(HashTable* table, char* key, char* value) {

    Ht_item* item = create_item(key, value);


    Ht_item* current_item = table->items[table->count];

    if (current_item == NULL) {
        // Key does not exist.
        if (table->count == table->size) {
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");
            // Remove the create item
            free_item(item);
            return;
        }
        
        // Insert directly
        table->items[table->count] = item; 
        table->count++;
        return;
    }

}

char* ht_search(HashTable* table, char* key) {

    //retrouver l'index de la clé. 
   int index = hash_index(table,key);
    Ht_item* item = table->items[index];
        

    // Ensure that we move to a non NULL item
    if (item != NULL) {
        if (strcmp(item->key, key) == 0){
            return item->value;
        }
    }
    return NULL;
}

void print_search(HashTable* table, char* key) {
    char* val=ht_search(table, key);
    if (val== NULL) {
        printf("Key %s does not exist\n", key);
        return;
    }
    else {
        printf("Key %s, Value %s\n", key, val);
    }
}

void print_table(HashTable* table) {
    printf("la taille du tableau est %u\n", table->size);
    for (int i=0; i<table->size; i++) {
        if (table->items[i]) {
            printf("Index:%d, %s -> %s\n", i, table->items[i]->key, table->items[i]->value);
        }
    }
}
int main(int argc, char ** argv){

    FILE *f;
    char l[BUFSIZE];
    char name[512];
    char lineage[512];
    HashTable *table= create_table(CAPACITY);

//vérif du document
    if ((f = fopen("taxo_species_linea.reduced", "r")) == NULL){
            fprintf(stderr,"Can't open taxo file");
    }
    if (fgets(l, BUFSIZE, f) == NULL){
        fprintf(stderr,"Empty taxo file");
    }

    while(fgets(l, BUFSIZE, f)!=NULL){
        sscanf(l,"%[^	]	%[^\n]", name, lineage);
       //printf(l,"%s : %s\n", name, lineage);
        ht_insert(table, name,lineage);
    }
    fclose(f);

    print_search(table,"Homo sapiens");
//    print_table(table);

    free_table(table);
    return 0;
}