#ifndef S_LINKED_LIST_H
#define S_LINKED_LIST_H

#include <stdbool.h>

/* Since I want the struct to be public to the API user, I place the definition here,
otherwise, I'd put it in the .c and just declare it in the .h file:
https://stackoverflow.com/questions/6316987/should-struct-definitions-go-in-h-or-c-file
*/
typedef struct SLinkedListNode{
    void* data;
    struct SLinkedListNode* next; 
} SLinkedListNode;

typedef struct SLinkedList{
    int n_elements;
    size_t data_size;
    SLinkedListNode* head;
    SLinkedListNode* tail;
} SLinkedList;

static SLinkedListNode* iterator;
/* Macro for iteration*/
#define LINKED_LIST_FOR_EACH(list, node) \
    iterator = list->head; \
    for(; iterator!= NULL; iterator=iterator->next)

/* Creators */
SLinkedList* SLinkedListCreate(int n_elements, size_t data_size);
SLinkedListNode* SLinkedListCreateNode(SLinkedList* list, void* data);

/* Adders */
void SLinkedListAddAtHead(SLinkedList* list, void* data);
void SLinkedListAddAtTail(SLinkedList* list, void* data);
bool SLinkedListAddAtIndex(SLinkedList* list, int index, void* data);

/* Deleters */
bool SLinkedListDeleteAtHead(SLinkedList* list);
bool SLinkedListDeleteAtTail(SLinkedList* list);
bool SLinkedListDeleteAtIndex(SLinkedList* list, int index);
void SLinkedListDelete(SLinkedList* list);

/* Getters */
SLinkedListNode* SLinkedListGetAtIndex(SLinkedList* list, int index);
SLinkedListNode* SLinkedListGetHead(SLinkedList* list);
SLinkedListNode* SLinkedListGetTail(SLinkedList* list);

/* Utils */
void reverse_linked_list(SLinkedListNode** head);
bool is_empty(SLinkedListNode* head);
int get_len(SLinkedListNode* head);

#endif