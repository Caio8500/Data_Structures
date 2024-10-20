#ifndef S_LINKED_LIST_H
#define S_LINKED_LIST_H

#include <stdbool.h>
/* Since I want the struct to be public to the API user, I place the definition here,
otherwise, I'd put it in the .c and just declare it in the .h file:
https://stackoverflow.com/questions/6316987/should-struct-definitions-go-in-h-or-c-file
*/
typedef struct SLinkedList{
    int val;
    struct SLinkedList* next; 
} SLinkedList;


SLinkedList* SLinkedListCreateNode(int val);
int SLinkedListDump(SLinkedList* head);
SLinkedList* SLinkedListGet(SLinkedList* head, int index);
SLinkedList* SLinkedListGetHead(SLinkedList* head);
SLinkedList* SLinkedListGetTail(SLinkedList* head);
void SLinkedListAddAtHead(SLinkedList** head, int val);
void SLinkedListDeleteAtHead(SLinkedList** head);
bool SLinkedListAddAtTail(SLinkedList* head, int val);
bool SLinkedListDeleteAtTail(SLinkedList* head);
bool SLinkedListAddAtIndex(SLinkedList* head, int index, int val);
bool SLinkedListDeleteAtIndex(SLinkedList* head, int index);
void reverse_linked_list(SLinkedList** head);
void SLinkedListFree(SLinkedList** head);
int is_cyclic(SLinkedList* head, int* cycle_len);
bool is_empty(SLinkedList* head);
int get_len(SLinkedList* head);
int find_intersection(SLinkedList* head_a, SLinkedList* head_b);
SLinkedList* SLinkedListShallowCopy(SLinkedList* orig);
SLinkedList* SLinkedListDeepCopy(SLinkedList* orig);
//int get_cycle_pos(SLinkedList* head, int len);

#endif