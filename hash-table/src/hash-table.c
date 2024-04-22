#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "hash-table.h"

/* Private */

struct Entry
{
  char *key;
  void *value;
  Entry *next;
};

struct HashTable
{
  uint32_t size;
  uint32_t initialSize;
  uint32_t count;
  Entry **elements;
  hashFunction *hash;
  double loadFactor;
  bool (*add)(HashTable *ht, const char *key, void *value);
  bool (*remove)(HashTable *ht, const char *key);
  void *(*get)(HashTable *ht, const char *key);
};

uint64_t djb2HashFunction(const char *key)
{
  uint64_t hashValue = 5381;

  int c;

  while ((c = *key++))
  {
    hashValue = ((hashValue << 5) + hashValue) + c; /* hash * 33 + c */
  }

  key -= 1;

  return hashValue;
}

void freeEntry(Entry *e)
{
  if (e == NULL)
    return;

  free(e->key);

  // if (e->value != NULL)
  //   free(e->value);

  freeEntry(e->next);

  free(e);
}

static void hashTableResize(HashTable *ht, uint32_t newSize)
{
  if (ht == NULL)
    return;

  uint32_t index = 0;

  Entry **newElements = calloc(newSize, sizeof(Entry));

  while (index <= ht->size)
  {
    Entry *e = ht->elements[index++];

    if (e != NULL)
      newElements[ht->hash(e->key) % newSize] = e;
  }

  free(ht->elements);

  ht->size = newSize;

  ht->elements = newElements;
}

static Entry *hashTableLookup(HashTable *ht, const char *key)
{
  Entry *e = ht->elements[ht->hash(key) % ht->size];

  while (e != NULL && strcmp(key, e->key) != 0)
    e = e->next;

  if (e == NULL)
    return NULL;

  return e;
}

/* Public functions */

void hashTableDestroy(HashTable *ht)
{
  uint32_t index = 0;

  while (index < ht->size)
    freeEntry(ht->elements[index++]);

  free(ht);
}

void hashTablePrint(HashTable *ht)
{
  uint32_t index = 0;

  printf("------ Hash Table ------\n");

  printf("count -> %d\n", ht->count);

  printf("size -> %d\n\n", ht->size);

  printf("------ Entries ------\n");

  while (index < ht->size)
  {
    Entry *e = ht->elements[index];

    if (e == NULL)
    {
      printf("key -> NULL, index -> %u\n", index);
    }
    else
    {
      printf("key -> %s, index -> %u\n", e->key, index);

      Entry *next = e->next;

      while (next != NULL)
      {
        printf("-- key -> %s, index -> %u\n", next->key, index);

        next = next->next;
      }
    }

    index++;

    if (index != ht->size)
      printf("=============\n");
  }

  printf("-------------\n\n");
}

void *hashTableGet(HashTable *ht, const char *key)
{
  Entry *e = hashTableLookup(ht, key);

  if (e == NULL)
    return NULL;

  return e->value;
}

bool hashTableAdd(HashTable *ht, const char *key, void *value)
{
  if (key == NULL || ht == NULL)
    return false;

  if (hashTableLookup(ht, key) != NULL)
    return false;

  Entry *e = malloc(sizeof(Entry));

  if (ht->size * ht->loadFactor < ht->count + 1)
    hashTableResize(ht, ht->size * 2);

  e->key = malloc(strlen(key) + 1);

  e->key = strdup(key);

  uint32_t index = ht->hash(key) % ht->size;

  e->value = value;

  e->next = ht->elements[index];

  ht->elements[index] = e;

  ht->count++;

  return true;
}

bool hashTableRemove(HashTable *ht, const char *key)
{
  if (ht == NULL || key == NULL)
    return false;

  if (hashTableLookup(ht, key) == NULL)
    return false;

  /* Only shrink when beneath current min and new size bigger then initial size */
  if ((ht->size / 2 * ht->loadFactor > ht->count - 1) && (ht->size / 2) >= ht->initialSize)
    hashTableResize(ht, ht->size / 2);

  uint32_t index = ht->hash(key) % ht->size;

  Entry *e = ht->elements[index];

  Entry *pe = NULL;

  while (e != NULL && strcmp(key, e->key) != 0)
  {
    pe = e;

    e = e->next;
  }

  if (e == NULL)
    return false;

  if (pe != NULL)
    pe->next = e->next;

  if (ht->elements[index] == pe)
    ht->elements[index] = pe;
  else
    ht->elements[index] = NULL;

  freeEntry(e);

  ht->count--;

  return true;
}
/*
  Creates a new HastTable
*/
HashTable *hashTableCreate(uint32_t size, hashFunction *hf)
{
  HashTable *ht = malloc(sizeof(HashTable));

  ht->elements = calloc(size, sizeof(Entry));

  ht->hash = hf == NULL ? djb2HashFunction : hf;

  ht->get = hashTableGet;

  ht->add = hashTableAdd;

  ht->remove = hashTableRemove;

  ht->size = size;

  ht->initialSize = size;

  ht->count = 0;

  ht->loadFactor = 0.75;

  return ht;
}

int main()
{
  HashTable *ht = hashTableCreate(5, NULL);

  ht->add(ht, "John", (int *)25);

  ht->add(ht, "Mary", (void *)30);

  ht->add(ht, "Eva", (void *)31);

  ht->add(ht, "Mike", "mike");
  ht->add(ht, "Jaap", NULL);
  ht->add(ht, "Kees", NULL);
  ht->add(ht, "Sofie", NULL);

  hashTablePrint(ht);

  ht->remove(ht, "John");

  hashTablePrint(ht);

  ht->remove(ht, "Kees");

  hashTablePrint(ht);

  ht->remove(ht, "Mike");

  hashTablePrint(ht);

  ht->remove(ht, "Eva");

  hashTablePrint(ht);

  ht->remove(ht, "Mary");

  hashTablePrint(ht);

  hashTableDestroy(ht);

  return 0;
}