#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t hashFunction(const char *key);

typedef struct Entry Entry;

typedef struct HashTable HashTable;

HashTable *hashTableCreate(uint32_t size, hashFunction *hf);
bool hashTableAdd(HashTable *ht, const char *key, void *value);
void *hashTableGet(HashTable *ht, const char *key);
bool hashTableRemove(HashTable *ht, const char *key);
void hashTableDestroy(HashTable *ht);
void hashTablePrint(HashTable *ht);

#endif