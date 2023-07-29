#include <stdlib.h>

/* Including own header for checking by compiler */
#define linkedList_IMPORT

#include "linked-list.h"

/* Private types */

/* Function prototypes */

static void linkedList_addNode(LinkedList *self, void *data);

static LinkedListNode *linkedList_nodeFactory(void *data);

/* Private functions */

static LinkedListNode *linkedList_nodeFactory(void *data)
{
  LinkedListNode *node = malloc(sizeof(LinkedListNode));

  node->next = NULL;

  node->data = data;

  return node;
}

static void linkedList_addNode(LinkedList *self, void *data)
{
  LinkedListNode *node = linkedList_nodeFactory(data);

  if (self->head == NULL)
    self->head = node;

  LinkedListNode *tail = self->tail;

  if (tail != NULL)
    tail->next = node;

  self->tail = node;

  self->count++;
}

/* Public functions */

LinkedList *linkedList_linkedListFactory()
{
  LinkedList *linkedList = malloc(sizeof(LinkedList));

  linkedList->head = NULL;

  linkedList->tail = NULL;

  linkedList->add = linkedList_addNode;

  linkedList->count = 0;

  return linkedList;
}