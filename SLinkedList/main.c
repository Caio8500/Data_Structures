/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include "sLinkedList.h"

int main(){
    
    /* testing basic functions */
    
    SLinkedList* test_list = NULL; // if we don't want to create an init node right away, 
                                   // test_list is equivalent to head_test, list and head are the same
    SLinkedListAddAtHead(&test_list, 10);
    SLinkedListAddAtTail(test_list, 20);
    SLinkedListAddAtTail(test_list, 40);
    SLinkedListAddAtTail(test_list, 80);
    SLinkedListAddAtTail(test_list, 160);
    SLinkedListDump(test_list);
    int index  = 5;
    SLinkedList* node = SLinkedListGet(test_list, 6);
    (node)?(printf("Getting element at index %d: val = %d\n", index, node->val)):(printf(""));
    
    index = 5;
    SLinkedListAddAtIndex(test_list, index, 88);
    SLinkedListDump(test_list);
    node = SLinkedListGet(test_list, index);
    (node)?(printf("Getting element at index %d: val = %d\n", index, node->val)):(printf(""));
    
    index = 6;
    SLinkedListDeleteAtIndex(test_list, index);
    SLinkedListDeleteAtTail(test_list);
    SLinkedListDump(test_list);
    node = SLinkedListGet(test_list, index);
    (node)?(printf("Getting element at index %d: val = %d\n", index, node->val)):(printf(""));
    
    SLinkedListDeleteAtHead(&test_list);
    SLinkedListAddAtIndex(test_list, 250, 320);
    SLinkedListAddAtIndex(test_list, 7, 740);
    SLinkedListDump(test_list);
    SLinkedListDeleteAtIndex(test_list, 6);
    SLinkedListDump(test_list);
    SLinkedListAddAtHead(&test_list, 1480);
    SLinkedListDump(test_list);
    printf("Current Head: %d\n", SLinkedListGetHead(test_list)->val);
    printf("Current Tail: %d\n", SLinkedListGetTail(test_list)->val);
    //int cyclic = is_cyclic(test_list);
    //(cyclic)?(printf("The list is cyclic\n")):(printf("The list is not cyclic\n"));

    SLinkedListFree(&test_list);
    SLinkedListDump(test_list);
    //printf("Getting element at index %d: val = %d\n", index, SLinkedListGet(test_list, index));
    
    /* testing cycle detection */
    
    //test_list = SLinkedListCreateNode();
    SLinkedListAddAtHead(&test_list, 3);
    SLinkedListAddAtTail(test_list, 23);
    SLinkedListAddAtTail(test_list, 33);
    SLinkedListAddAtTail(test_list, 43);
    SLinkedListAddAtTail(test_list, 53);
    SLinkedListAddAtTail(test_list, 43);
    //SLinkedListAddAtTail(test_list, 83);
    SLinkedListAddAtTail(test_list, 163);
    SLinkedListDump(test_list);
    // hardcoding a cycle inside the list to element at index 1
    //SLinkedListGetTail(test_list)->next = SLinkedListGetHead(test_list)->next;
    
    SLinkedList* list_d = SLinkedListDeepCopy(test_list);
    SLinkedListAddAtTail(list_d, 111);
    printf("List d deep copy of test list before cycle: \n");
    // checking that list d is indeed a deep copy of test_list
    SLinkedListDump(list_d);
    SLinkedListDump(test_list);
    
    // creating cycle inside list
    SLinkedListGetTail(test_list)->next = SLinkedListGetHead(test_list)->next->next;
    //printf("List len: %d\n", get_len(test_list));
    //int cyclic = is_cyclic(test_list);
    //(cyclic)?(printf("The list is cyclic\n")):(printf("The list is not cyclic\n"));
    SLinkedListDump(test_list);
    
    SLinkedList* list_e = SLinkedListDeepCopy(test_list);
    SLinkedListAddAtTail(list_e, 111);
    printf("List e deep copy of test list after cycle: \n");
    // checking that list d is indeed a deep copy of test_list
    SLinkedListDump(list_e);
    SLinkedListDump(test_list);
    
    /* testing intersection */
    
    SLinkedList* list_a = SLinkedListCreateNode(1);
    
    SLinkedListAddAtHead(&list_a, 11);
    SLinkedListAddAtTail(list_a, 21);
    SLinkedListAddAtTail(list_a, 41);
    SLinkedListAddAtTail(test_list, 66); 
    SLinkedListAddAtIndex(test_list, 4, 66);
    SLinkedListGetTail(list_a)->next = SLinkedListGetHead(test_list); // joining list_a with test_list
    SLinkedListDump(list_a);
    
    SLinkedList* list_b = SLinkedListShallowCopy(test_list); // list b is now test_list
    SLinkedListAddAtIndex(list_b, 2, 222);
    
    /*SLinkedList* list_b = SLinkedListCreateNode(2);
    
    SLinkedListAddAtHead(&list_b, 11);
    SLinkedListAddAtTail(list_b, 22);
    SLinkedListAddAtTail(list_b, 42);
    SLinkedListAddAtTail(list_b, 82);
    SLinkedListAddAtTail(list_b, 162);
    SLinkedListGetTail(list_b)->next = SLinkedListGetHead(test_list);
    */
    printf("list b shallow copy of test list: \n");
    SLinkedListDump(list_b);
    SLinkedListDump(test_list);
    
    int intersection = find_intersection(list_a, list_b); // if list b is a shallow copy, the intersection will be at the first position
    (intersection>=0)?(printf("Lists intersect at position %d of the first list\n", intersection)):(printf("The lists do not intersect\n"));
  
    /* testing reversing */
    // SLinkedList* list_c = SLinkedListCreateNode(1);
    SLinkedList* list_c = NULL;
    // two ways of adding a head to an initially empty list
    // list_c = SLinkedListCreateNode(1);
    SLinkedListAddAtHead(&list_c,1);
    SLinkedListAddAtTail(list_c, 2);
    SLinkedListAddAtTail(list_c, 3);
    SLinkedListAddAtTail(list_c, 4);
    SLinkedListDump(list_c);
    /* testing reversing, for this we can either return a pointer to the head (which makes sense to make a copy but it would still be a shallow copy 
    as no new malloc has been made and the copy head pointer would point to the same memory zones of the previous list, so (in my opinion) it makes more sense to 
    change inside the function the value of the address pointed by the head pointer*/
    reverse_linked_list(&list_c); 
    SLinkedListDump(list_c);
    //int get_cycle_pos(SLinkedList* head, int len);
}