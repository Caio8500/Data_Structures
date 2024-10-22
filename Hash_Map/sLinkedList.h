#ifndef S_LINKED_LIST_H
#define S_LINKED_LIST_H

#include <stdbool.h>
/* Since I want the struct to be public to the API user, I place the definition here,
otherwise, I'd put it in the .c and just declare it in the .h file:
https://stackoverflow.com/questions/6316987/should-struct-definitions-go-in-h-or-c-file
*/

typedef struct Data{
    int key; 
    int val;
} Data;

typedef struct SLinkedList{
    Data kv_pair;
    struct SLinkedList* next; 
} SLinkedList;


SLinkedList* SLinkedListCreateNode(Data kv_pair);
int SLinkedListDump(SLinkedList* head);
SLinkedList* SLinkedListGet(SLinkedList* head, int index);
SLinkedList* SLinkedListGetByKey(SLinkedList* head, int key);
SLinkedList* SLinkedListGetUpdateByKey(SLinkedList* head, Data kv_pair);
SLinkedList* SLinkedListGetHead(SLinkedList* head);
SLinkedList* SLinkedListGetTail(SLinkedList* head);
void SLinkedListAddAtHead(SLinkedList** head, Data kv_pair);
void SLinkedListDeleteAtHead(SLinkedList** head);
bool SLinkedListAddAtTail(SLinkedList* head, Data kv_pair);
bool SLinkedListDeleteAtTail(SLinkedList* head);
bool SLinkedListAddAtIndex(SLinkedList* head, int index, Data kv_pair);
bool SLinkedListDeleteAtIndex(SLinkedList* head, int index);
bool SLinkedListDeleteByKey(SLinkedList** head, int index);
void SLinkedListReverse(SLinkedList** head);
void SLinkedListFree(SLinkedList** head);
int SLinkedListIsCyclic(SLinkedList* head, int* cycle_len);
int SLinkedListGetLen(SLinkedList* head);
int SLinkedListFindIntersection(SLinkedList* head_a, SLinkedList* head_b);
SLinkedList* SLinkedListShallowCopy(SLinkedList* orig);
SLinkedList* SLinkedListDeepCopy(SLinkedList* orig);
//int get_cycle_pos(SLinkedList* head, int len);

#endif