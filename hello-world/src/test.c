#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LinkedList LinkedList;

typedef struct LinkedListNode LinkedListNode;

typedef struct LinkedListNode {
  void *data;
  LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedList {
  LinkedListNode *head;
  LinkedListNode *tail;
  int count;
  void (*add)(LinkedList *self, void *data);
} LinkedList;

void addLinkedListNode(LinkedList *self, void *data);

LinkedListNode* linkedListNodeFactory(void *data);

LinkedList* linkedListFactory();

LinkedList* linkedListFactory() {
  LinkedList *linkedList = malloc(sizeof(LinkedList));

  linkedList->head = NULL;

  linkedList->tail = NULL;

  linkedList->add = addLinkedListNode;

  linkedList->count = 0;

  return linkedList;
}

LinkedListNode* linkedListNodeFactory(void *data) {
  LinkedListNode *node = malloc(sizeof(LinkedListNode));

  node->next = NULL;

  node->data = data;

  return node;
}


void addLinkedListNode(LinkedList *self, void *data) {
  LinkedListNode *node = linkedListNodeFactory(data);

  if (self->head == NULL) self->head = node;

  LinkedListNode *tail = self->tail;

  if (tail != NULL) tail->next = node;

  self->tail = node;

  self->count++;
}

int main() {
  LinkedList *list = linkedListFactory();

  list->add(list, "Hello");

  printf("head: %s\n", (char*)list->head->data);

  list->add(list, "Bye");

  printf("tail: %s\n", (char*)list->tail->data);

  printf("count: %d\n", list->count);

  LinkedListNode * nodes = malloc(2* sizeof(LinkedListNode));

  nodes[0] = *linkedListNodeFactory("hoi");

  nodes[1] = *linkedListNodeFactory("doei");

  printf("size: %lu", sizeof(nodes));

  int index = 0;

  while(index < 10) {
   LinkedListNode node = *nodes++;

   printf("node: %s\n", (char*)node.data);

   index++;
  }

  return 1;
}