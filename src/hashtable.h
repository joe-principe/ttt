#ifndef HASHTABLE_H
#define HASHTABLE_H

#define TABLE_SIZE 500000

typedef struct entry_t entry_t;
typedef struct ht_t ht_t;

struct entry_t
{
    char *key;
    char *value;
    struct entry_t *next;
};

struct ht_t
{
    entry_t **entries;
};

unsigned int hash(const char *key);

ht_t *ht_create(void);

entry_t *ht_pair(const char *key, const char *value);

void ht_set(ht_t *hash_table, const char *key, const char *value);

char *ht_get(const ht_t *hash_table, const char *key);

void ht_dump(const ht_t *hash_table);

#endif
/* EOF */
