#ifndef linked_list_H
#define linked_list_H

/* Set EXTERN macro */
#ifdef linkedList_IMPORT
#define LINKED_LIST_EXTERN
#else
#define LINKED_LIST_EXTERN extern
#endif

/* Type declarations */

typedef struct LinkedList LinkedList;

typedef struct LinkedListNode LinkedListNode;

typedef struct LinkedListNode
{
  void *data;
  LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedList
{
  LinkedListNode *head;
  LinkedListNode *tail;
  int count;
  void (*add)(LinkedList *self, void *data);
  void (*free)(LinkedList *self);
} LinkedList;

/* Function prototypes */

LinkedList *linkedList_linkedListFactory();

#undef linkedList_IMPORT
#undef LINKED_LIST_EXTERN
#endif