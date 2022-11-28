#include <stdio.h>
#include "dict.h"

/* Macro to declare dict structure for type int */
DECLARE_DICT(int)
DEFINE_DICT(int)

int main() {
        // Each function include the type in its name (here dict_int for int)
        dict_int *d = dict_int_init();
        char key[8];

        for (int i=0; i<1024; ++i) {
                sprintf(key, "%d", i);
                // The key is duplicated when set
                dict_int_set(d, key, i);
        }
        if (dict_int_exists(d, "12"))
                puts("Key 12 is present");

        bool present;
        int val = dict_int_get(d, "1000", &present);
        if (present)
                printf("Key 1000 has value: %d\n", val);

        bool was_deleted = dict_int_unset(d, "1000");
        if (was_deleted)
                puts("Key 1000 was actually found and deleted");

        dict_int_free(d);
}