/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "sLinkedList.h"
/*
quick reminder on the build process, during linkage, the compiler will assigne the definition of functions
and types (.h) to an implementation (.c), it is at this point where we can tweak the build process in order 
to change the implementation of a given function, this is specially useful for abstraction layers such as an OSAL
*/

/*
notes on creation of this linked list:
for an empty list, just point the head straight to NULL, if you do so and 
try to add to the tail, it'll fail as this function calls head->next which throws 
a runtime error, the user must, in that situation, add a head to the list either 
with the SLinkedListAddAtHead function ot with the SLinkedListCreateNode function.
If the user chooses not to create a node at declaration time, it must be set to NULL,
as the function of traversing and addition to the list depend on the NULL nodes 
being well-defined.
It is important to notice the subtle difference between list and node creation,
the list creation might involve the creation of a node right away or not, 
it can just create the list (the HEAD pointer). This shows that there is no point
in having different methods for node creation as it doesn't matter if the node is the head 
or not
*/

/* private functions that won't be defined on the header file in order to hide them from the user */
int calculate_cycle_len(SLinkedList* node);
int get_cycle_pos(SLinkedList* head, int len);

SLinkedList* SLinkedListCreateNode(Data kv_pair) {
    SLinkedList* new_node = malloc(sizeof(SLinkedList));
    new_node->next = NULL;
    memcpy(&(new_node->kv_pair), &kv_pair, sizeof(Data));
    return new_node;
}

int SLinkedListDump(SLinkedList* head){
    if(head == NULL){
        printf("*** %s Warning: The list is empty, add a head to it! ***\n", __func__);
        return 1; // empty list
    }

    int hops_counter = 0;
    int cycle_pos, cycle_len;
    // the method to loop over a cyclic list is different than a regular list as the tail does not point to NULL
    printf("# Linked List Dump #\n--------------------- \n");
    if((cycle_pos = SLinkedListIsCyclic(head, &cycle_len))>= 0){
        printf("*** Warning: Cyclic list! cycle len: %d; cycle pos: %d ***\n", cycle_len, cycle_pos);
        while(hops_counter < (cycle_len + cycle_pos)){
            printf("(%d) -> addr: %p; key: %d; val: %d; next: %p\n", hops_counter, head, head->kv_pair.key, head->kv_pair.val, head->next);
            head = head->next;
            hops_counter++;
        }
        return 0; // we have reached the last node before the loop
    }
    else{
        while(head != NULL){
            printf("(%d) -> addr: %p; key: %d; val: %d; next: %p\n", hops_counter, head, head->kv_pair.key, head->kv_pair.val, head->next);
            head = head->next;
            hops_counter++;
        }
    }
    printf("--------------------- \n");
    return 0;
    
}

SLinkedList* SLinkedListGet(SLinkedList* head, int index) {
    for(int i = 0; i<index; i++){
        // we must stop at the index prior to the target in order to rearrange the 'next' pointers
        head = head->next;
        if (head == NULL){
            // if we're at the tail
            printf("Get at index: index %d is invalid!\n", index);
            return  NULL;
        }
    }
    return head;
}

SLinkedList* SLinkedListGetByKey(SLinkedList* head, int key) {
    while(head != NULL){
        // we have to go all the way to the tail
        if(head->kv_pair.key == key){
            return head;
        }
        head = head->next;
    }
    printf("*** %s: key %d is unavailable! ***\n", __func__, key);
    return NULL;
}

SLinkedList* SLinkedListGetUpdateByKey(SLinkedList* head, Data kv_pair) {
    while(head != NULL){
        if(head->kv_pair.key == kv_pair.key){
            head->kv_pair.val = kv_pair.val;
            return head;
        }
        head = head->next;
    }
    printf("*** %s: Updating key %d with value %d... ***\n", __func__, kv_pair.key, kv_pair.val);
    return NULL;
}

SLinkedList* SLinkedListGetHead(SLinkedList* head){
    return SLinkedListGet(head, 0);
}

SLinkedList* SLinkedListGetTail(SLinkedList* head){
    while(head->next != NULL){
        head = head->next;
    }
    return head;
}

void SLinkedListAddAtHead(SLinkedList** head, Data kv_pair) {
    // it is necessary to pass a pointer to a pointer here as we want to change the reference of the list's head
    SLinkedList* new_node = SLinkedListCreateNode(kv_pair);
    new_node->next = *head; // the new node is going to be the new head, so the current head will be its next element
    *head = new_node; // head now points at new node
}

void SLinkedListDeleteAtHead(SLinkedList** head) {
    // it is necessary to pass a pointer to a pointer here as we want to change the reference of the list's head
    SLinkedList* to_be_deleted = *head;
    *head = (*head)->next; // head now points at second node
    free(to_be_deleted); // deletion of previous head
}

bool SLinkedListAddAtTail(SLinkedList* head, Data kv_pair) {
    if(head == NULL){
        printf("*** %s Warning: The list is empty, add a head to it! ***\n", __func__);
        return 1;
    }
    SLinkedList* new_node = SLinkedListCreateNode(kv_pair);
    if(SLinkedListIsCyclic(head, NULL) >= 0){
        // -1 will also return true as it is different than 0
        printf("Can't add element %d: the list is cyclic, it has no tail! \n", kv_pair.val);
        return 1;
    }
    while(head->next != NULL){
        head = head->next;
    }
    head->next = new_node;
    return 0;
}

bool SLinkedListDeleteAtTail(SLinkedList* head) {
    if(head == NULL){
        printf("*** %s Warning: The list is empty, add a head to it! ***\n", __func__);
        return 1;
    }
    if(SLinkedListIsCyclic(head, NULL) >= 0){
        // -1 will also return true as it is different than 0
        printf("Can't delete tail: the list is cyclic, it has no tail! \n");
        return 1;
    }
    while(head->next->next != NULL){
        head = head->next;
    }
    SLinkedList* to_be_deleted = head->next;
    head->next = NULL;
    free(to_be_deleted); // deletion of previous head

    return 0;
}

bool SLinkedListAddAtIndex(SLinkedList* head, int index, Data kv_pair) {
    SLinkedList* new_node = SLinkedListCreateNode(kv_pair);
    if(index == 0){
        printf("To add nodes to the head of the list, use the SLinkedListAddAtHead function \n");
        return 1;
    }
    for(int i = 0; i<index-1; i++){
        // we must stop at the index prior to the target in order to rearrange the 'next' pointers
        if (head->next == NULL){
            // if it is the tail
            printf("Delete at index: index %d is invalid!\n", index);
            return 1;
        }
        head = head->next;
    }
    // shifting 'next' pointers here 
    new_node->next = head->next;
    head->next  = new_node;
    return 0;
}

bool SLinkedListDeleteAtIndex(SLinkedList* head, int index) {
    if(index == 0){
        printf("To delete the head of the list, use the SLinkedListDeleteAtHead function \n");
        return 1;
    }
    for(int i = 0; i<index-1; i++){
        // we must stop at the index prior to the target in order to rearrange the 'next' pointers
        head = head->next;
        if (head->next == NULL){
            printf("Delete at index: index %d is invalid!\n", index);
            return 1;
        }
    }
        
    SLinkedList* to_be_deleted = head->next;
    head->next  = head->next->next;
    // freeing up memory allocated by deleted node
    free(to_be_deleted);
    
    return 0;
}

bool SLinkedListDeleteByKey(SLinkedList** head, int key){
    // add support to head deletion
    SLinkedList* current_node = *head; // careful not to change the head as we pass the head pointer's address
    int aux_idx = 0;
    
    while(current_node != NULL){
        if(current_node->next->kv_pair.key == key){
            SLinkedList* to_be_deleted = current_node->next;
            current_node->next  = current_node->next->next;
            // freeing up memory allocated by deleted node
            free(to_be_deleted);
            return true;
        }
        if(aux_idx == 0){
            // edge case for list head
            if(current_node->kv_pair.key == key){
                SLinkedListDeleteAtHead(head);
                return true;
            }
        }
        current_node = current_node->next;
        aux_idx++;
    }
    return false;
}

/*
void reverse_linked_list(SLinkedList** head){
    int list_len = SLinkedListGetLen(*head);
    SLinkedList* temp_next = NULL;
    SLinkedList* temp_prev = NULL;
    
    for(int i = 0; i<list_len; i++){
        temp_next = (*head)->next; // save the next element before changing next pointers
        if(i == 0){
            // edge case of first node
            (*head)->next = NULL;
        }
        else{
            (*head)->next = temp_prev;
        }
        temp_prev = *head;
        if(temp_next != NULL){
            *head = temp_next;
        }
    }
}
*/

void SLinkedListReverse(SLinkedList** head){
    
    SLinkedList* temp_next = NULL;
    SLinkedList* temp_prev = NULL;
    
    while((*head) != NULL){
        
        temp_next = (*head)->next; // save the next element before changing next pointers
        (*head)->next = temp_prev; // no need to check if it is the first element as temp_prev starts at NULL
        temp_prev = *head;
        *head = temp_next;
    }
    // the while loop will inevitably set HEAD to NULL, so we need to reset HEAD to prev
    *head = temp_prev;
    
}

void SLinkedListFree(SLinkedList** head) {
    SLinkedList* to_be_deleted = NULL;
    
    while((*head)->next != NULL){
        to_be_deleted = *head;
        *head = (*head)->next;
        free(to_be_deleted);
    }
    *head = NULL; // marking the list as empty
}

int calculate_cycle_len(SLinkedList* node){
    SLinkedList* starting_point = node;
    int len = 0;
    do{
        len++;
        node = node->next;
    }
    while(node != starting_point); // loop over the list until we meet the slow iterator again
    return len;
}

int get_cycle_pos(SLinkedList* head, int len){
    SLinkedList* iterator = head;
    int hops_counter = 0; // number of next calls
    int pos = 0;
    do{
        if(hops_counter > len){
            // means that the list is not cyclic for current node
            head = head->next; // we'll see if the next node is cyclic or not
            iterator = head;
            pos++;
            hops_counter = 0;
        }
        hops_counter++;
        iterator = iterator->next;
    }
    while(head != iterator); // using do whiles so that the first iteration is executed without having to call the next node right away
    return pos;
}

int SLinkedListIsCyclic(SLinkedList* head, int* cycle_len){
    SLinkedList* slow_iterator = head;
    SLinkedList* fast_iterator = head;
    int cycle_pos = 0;
    // using the two pointer technique to determine whether the linked list is cyclic or not
    // https://github.com/Chanda-Abdul/Several-Coding-Patterns-for-Solving-Data-Structures-and-Algorithms-Problems-during-Interviews/blob/main/%E2%9C%85%20%20Pattern%2003:%20Fast%20%26%20Slow%20pointers.md
    while(fast_iterator != NULL && fast_iterator->next != NULL){
        slow_iterator = slow_iterator->next;
        fast_iterator = fast_iterator->next->next;
        if(slow_iterator == fast_iterator){
            // if we have reached this condition, it means that the list is guaranteed to be cyclic and we can calculate the cycle's length using the slow_iterator
            if(cycle_len != NULL){
                *cycle_len = calculate_cycle_len(slow_iterator); 
                cycle_pos = get_cycle_pos(head, *cycle_len);
            }
            else{
                // if we don't care about getting the lenght
                cycle_pos = get_cycle_pos(head, calculate_cycle_len(slow_iterator));
            }
            //printf("Cycle length: %d; Cycle pos: %d\n", *cycle_len, cycle_pos);
            return cycle_pos; // the list is cyclic
        }
    }
    return -1; // the list is not cyclic
}

int SLinkedListGetLen(SLinkedList* head){
    
    int cycle_pos, cycle_len;
    int hops_counter = 0;
    
    // if list is cyclic
    if((cycle_pos = SLinkedListIsCyclic(head, &cycle_len))>= 0){
        while(hops_counter < (cycle_len + cycle_pos)){
            head = head->next;
            hops_counter++;
        }
    }
    else{
        while(head != NULL){
            head = head->next;
            hops_counter++;
        }
    }
    return hops_counter;
}

int SLinkedListFindIntersection(SLinkedList* head_a, SLinkedList* head_b){
    int len_a = SLinkedListGetLen(head_a);
    int len_b = SLinkedListGetLen(head_b);
    int intersect_pos = 0; // the position will be returned from the perspective of list A
    // we'll advance the bigger list to the point where they are both equal and then we'll start to compare their elements
    if(len_a>len_b){
        for(int i = 0; i < (len_a - len_b); i++){
            /* just so that i don't forget, this is an example of locally changing the reference of a pointer, 
            we change the reference of the head inside the function, but externally the head reference is protected 
            because the head pointer is not passed by reference ** head_a,
            but by value, we are passing the the address of the struct stored inside the pointer and not the address of the pointer
            itself
            */
            head_a = head_a->next; 
            intersect_pos++;
        }
    }
    else{
        for(int i = 0; i < (len_b - len_a); i++){
            head_b = head_b->next; 
        }
    }
    // now that they have both the same number of elements, we can compare them one by one, 
    while(head_a->next != NULL){
        if(head_a == head_b){
            return intersect_pos;
        }
        head_a = head_a->next;
        head_b = head_b->next;
        intersect_pos++;
    }
    // edge case of first elements intersecting
    if(head_a == head_b){
            return intersect_pos;
    }
    return -1; // do not intersect
}

SLinkedList* SLinkedListShallowCopy(SLinkedList* orig){
    /* the copy will be shallow as no new memory will be allocated, 
       it will just point to the same memory location of the original list,
       meaning that any changes either on the copy or on the original will 
       affect both lists */
     return SLinkedListGetHead(orig);  
}

SLinkedList* SLinkedListDeepCopy(SLinkedList* orig){
    // create list head with information from the original list
    SLinkedList* copy = SLinkedListCreateNode(orig->kv_pair);

    int hops_counter = 0;
    int cycle_pos, cycle_len;
    
    if((cycle_pos = SLinkedListIsCyclic(orig, &cycle_len))>= 0){
        printf("*** Warning: Cyclic list! cycle len: %d; cycle pos: %d ***\n", cycle_len, cycle_pos);
        while(hops_counter < (cycle_len + cycle_pos) - 1){
            orig = orig->next;
            hops_counter++;
            SLinkedListAddAtTail(copy, orig->kv_pair);
        }
        
        // creating cycle on copied list
       SLinkedListGetTail(copy)->next = SLinkedListGet(copy, cycle_pos);
    }
    else{
        while(orig->next != NULL){
            
            orig = orig->next; // since we've already copied the head at creation, we first advance the list 
            SLinkedListAddAtTail(copy, orig->kv_pair); // malloc a new element which takes the value of the current origin list,
                                                   // points head->next to the new elem and new_elem->next to NULL
        }
    }
    return copy;
}







