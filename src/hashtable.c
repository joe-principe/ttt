#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

unsigned int
hash(const char *key)
{
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    for (i = 0; i < key_len; i++) {
        value = value * 37 + key[i];
    } /* for */

    value = value % TABLE_SIZE;

    return value;
}

ht_t *
ht_create(void)
{
    int i = 0;
    ht_t *hash_table = malloc(sizeof(ht_t));

    hash_table->entries = malloc(sizeof(entry_t*) * TABLE_SIZE);
    
    for (i = 0; i < TABLE_SIZE; i++) {
        hash_table->entries[i] = NULL;
    } /* for */

    return hash_table;
}

entry_t *
ht_pair(const char *key, const char *value)
{
    entry_t *entry = malloc(sizeof(entry_t));
    entry->key = malloc(strlen(key) + 1);
    entry->value = malloc(strlen(key) + 1);

    strcpy(entry->key, key);
    strcpy(entry->value, value);

    entry->next = NULL;

    return entry;
}

void
ht_set(ht_t *hash_table, const char *key, const char *value)
{
    unsigned int slot = hash(key);
    entry_t *entry = hash_table->entries[slot];
    entry_t *prev = NULL;

    if (entry == NULL) {
        hash_table->entries[slot] = ht_pair(key, value);
        return;
    } /* if */

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            free(entry->value);
            entry->value = malloc(strlen(value) + 1);
            strcpy(entry->value, value);
            return;
        } /* if */
        prev = entry;
        entry = prev->next;
    } /* while */

    prev->next = ht_pair(key, value);
}

char *
ht_get(const ht_t *hash_table, const char *key)
{
    unsigned int slot = hash(key);
    entry_t *entry = hash_table->entries[slot];

    if (entry == NULL) { return NULL; }

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) { return entry->value; }
        entry = entry->next;
    } /* while */

    return NULL;
}

void
ht_dump(const ht_t *hash_table)
{
    int i;
    entry_t *entry = NULL;

    for (i = 0; i < TABLE_SIZE; i++) {
        entry = hash_table->entries[i];

        if (entry == NULL) { continue; }

        printf("slot[%4d]: ", i);

        while (1) {
            printf("%s = %s ", entry->key, entry->value);

            if (entry->next == NULL) { break; }
            
            entry = entry->next;
        } /* while */

        printf("\n");
    } /* for */
}
/* EOF */
