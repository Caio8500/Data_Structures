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
int calculate_cycle_len(SLinkedListNode* node);
int get_cycle_pos(SLinkedListNode* head, int len);


/* Creators */
SLinkedList* SLinkedListCreate(int n_elements, size_t data_size){
    SLinkedList* new_list = malloc(sizeof(SLinkedList));
    new_list->n_elements = n_elements;
    new_list->data_size = data_size;
    new_list->head = NULL;
    new_list->tail = NULL;
    return new_list;
}


SLinkedListNode* SLinkedListCreateNode(SLinkedList* list, void* data) {
    SLinkedListNode* new_node = malloc(sizeof(SLinkedListNode));
    new_node->next = NULL;
    new_node->data = malloc(list->data_size);
    memcpy(new_node->data, data, list->data_size); // try my implementation later
    return new_node;
}

/* Adders */
void SLinkedListAddAtHead(SLinkedList* list, void* data) {
    SLinkedListNode* new_node = SLinkedListCreateNode(list, data);
    new_node->next = list->head; // new head->next points to previous head
    list->head = new_node;
    if (list->head->next == NULL)
        list->tail = list->head;
}

void SLinkedListAddAtTail(SLinkedList* list, void* data) {
    if(list->head == NULL)
        printf("*** %s Warning: The list is empty, adding node at head... ***\n", __func__);
    SLinkedListNode* new_node = SLinkedListCreateNode(list, data);
    SLinkedListNode* current_node = list->head; // we have to use a local clone here in order to not unwillingly alter the head of the list since we pass it by ref
    while(current_node->next != NULL){
        current_node = current_node->next;
    }
    current_node->next = new_node;
    list->tail = new_node;
}

bool SLinkedListAddAtIndex(SLinkedList* list, int index, void* data) {
    SLinkedListNode* new_node = SLinkedListCreateNode(list, data);
    if(index == 0){
        SLinkedListAddAtHead(list, data);
        return 0;
    }
    SLinkedListNode* current_node = list->head;
    for(int i = 0; i<index-1; i++){
        // we must stop at the index prior to the target in order to rearrange the 'next' pointers
        if (current_node->next == NULL){
            // if we're at the tail and the for loop is not done yet
            printf("%s: index %d is invalid!\n",__func__, index);
            return 1;
        }
        current_node = current_node->next;
    }
    // shifting 'next' pointers here 
    new_node->next = current_node->next;
    current_node->next  = new_node;
    if(current_node->next == NULL){
        list->tail = current_node;
    }
    return 0;
}

/* Deleters */
bool SLinkedListDeleteAtHead(SLinkedList* list) {
    // it is necessary to pass a pointer to a pointer here as we want to change the reference of the list's head
    if(list->head == NULL){
        printf("*** %s Warning: The list is empty, nothing to delete! ***\n", __func__);
        return 1;
    }
    SLinkedListNode* to_be_deleted = list->head;
    list->head = list->head->next;
    free(to_be_deleted->data);
    free(to_be_deleted); // deletion of previous head
    return 0;
}

bool SLinkedListDeleteAtTail(SLinkedList* list) {
    if(list->head == NULL){
        printf("*** %s Warning: The list is empty, nothing to delete! ***\n", __func__);
        return 1;
    }
    SLinkedListNode* current_node = list->head;
    while(current_node->next->next != NULL){
        current_node = current_node->next;
    }
    SLinkedListNode* to_be_deleted = current_node->next;
    current_node->next = NULL;
    list->tail = current_node;
    free(to_be_deleted->data);
    free(to_be_deleted);

    return 0;
}

bool SLinkedListDeleteAtIndex(SLinkedList* list, int index) {
    if(index == 0){
        SLinkedListDeleteAtHead(list);
        return 0;
    }
    SLinkedListNode* current_node = list->head;
    for(int i = 0; i<index-1; i++){
        // we must stop at the index prior to the target in order to rearrange the 'next' pointers
        current_node = current_node->next;
        if (current_node->next == NULL){
            printf("Delete at index: index %d is invalid!\n", index);
            return 1;
        }
    }
        
    SLinkedListNode* to_be_deleted = current_node->next;
    // shifting 'next' pointers here
    current_node->next  = current_node->next->next;
    // freeing up memory allocated by to be deleted node
    free(to_be_deleted->data);
    free(to_be_deleted);
    
    return 0;
}

void SLinkedListDelete(SLinkedList* list) {
    SLinkedListNode* to_be_deleted = NULL;
    if(list == NULL){
        return;
    }
    SLinkedListNode* current_node = list->head;
    while(current_node!= NULL){
        to_be_deleted = current_node;
        current_node = (current_node)->next;
        free(to_be_deleted->data);
        free(to_be_deleted);
    }
    free(list);
    list->head = NULL; // marking the list as empty
}

/* Getters */
SLinkedListNode* SLinkedListGetAtIndex(SLinkedList* list, int index) {
    SLinkedListNode* current_node = list->head;
    if (index < 0){
        return NULL;
    }
    for(int i = 0; i<index; i++){
        // we must stop at the index prior to the target in order to rearrange the 'next' pointers
        current_node = current_node->next;
        if (current_node == NULL){
            // if it is the tail
            return  NULL;
        }
    }
    return current_node;
}

SLinkedListNode* SLinkedListGetHead(SLinkedList* list){
    return list->head;
}

SLinkedListNode* SLinkedListGetTail(SLinkedList* list){
    return list->tail;
}

#ifdef comment
int SLinkedListDump(SLinkedListNode* head){
    if(head == NULL){
        printf("*** %s Warning: The list is empty, add a head to it! ***\n", __func__);
        return 1; // empty list
    }

    int hops_counter = 0;
    int cycle_pos, cycle_len;
    // the method to loop over a cyclic list is different than a regular list as the tail does not point to NULL
    printf("# Linked List Dump #\n--------------------- \n");
    if((cycle_pos = is_cyclic(head, &cycle_len))>= 0){
        printf("*** Warning: Cyclic list! cycle len: %d; cycle pos: %d ***\n", cycle_len, cycle_pos);
        while(hops_counter < (cycle_len + cycle_pos)){
            printf("(%d) -> addr: %p; val: %d; next: %p\n", hops_counter, head, head->val, head->next);
            head = head->next;
            hops_counter++;
        }
        return 0; // we have reached the last node before the loop
    }
    else{
        while(head != NULL){
            printf("(%d) -> addr: %p; val: %d; next: %p\n", hops_counter, head, head->val, head->next);
            head = head->next;
            hops_counter++;
        }
    }
    printf("--------------------- \n");
    return 0;
    
}


/*
void reverse_linked_list(SLinkedListNode** head){
    int list_len = get_len(*head);
    SLinkedListNode* temp_next = NULL;
    SLinkedListNode* temp_prev = NULL;
    
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

void reverse_linked_list(SLinkedListNode** head){
    
    SLinkedListNode* temp_next = NULL;
    SLinkedListNode* temp_prev = NULL;
    
    while((*head) != NULL){
        
        temp_next = (*head)->next; // save the next element before changing next pointers
        (*head)->next = temp_prev; // no need to check if it is the first element as temp_prev starts at NULL
        temp_prev = *head;
        *head = temp_next;
    }
    // the while loop will inevitably set HEAD to NULL, so we need to reset HEAD to prev
    *head = temp_prev;
    
}


int calculate_cycle_len(SLinkedListNode* node){
    SLinkedListNode* starting_point = node;
    int len = 0;
    do{
        len++;
        node = node->next;
    }
    while(node != starting_point); // loop over the list until we meet the slow iterator again
    return len;
}

int get_cycle_pos(SLinkedListNode* head, int len){
    SLinkedListNode* iterator = head;
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

int is_cyclic(SLinkedListNode* head, int* cycle_len){
    SLinkedListNode* slow_iterator = head;
    SLinkedListNode* fast_iterator = head;
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

int get_len(SLinkedListNode* head){
    
    int cycle_pos, cycle_len;
    int hops_counter = 0;
    
    // if list is cyclic
    if((cycle_pos = is_cyclic(head, &cycle_len))>= 0){
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

int find_intersection(SLinkedListNode* head_a, SLinkedListNode* head_b){
    int len_a = get_len(head_a);
    int len_b = get_len(head_b);
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

SLinkedListNode* SLinkedListShallowCopy(SLinkedListNode* orig){
    /* the copy will be shallow as no new memory will be allocated, 
       it will just point to the same memory location of the original list,
       meaning that any changes either on the copy or on the original will 
       affect both lists */
     return SLinkedListGetHead(orig);  
}

SLinkedListNode* SLinkedListDeepCopy(SLinkedListNode* orig){
    // create list head with information from the original list
    SLinkedListNode* copy = SLinkedListCreateNode(orig->val);

    int hops_counter = 0;
    int cycle_pos, cycle_len;
    
    if((cycle_pos = is_cyclic(orig, &cycle_len))>= 0){
        printf("*** Warning: Cyclic list! cycle len: %d; cycle pos: %d ***\n", cycle_len, cycle_pos);
        while(hops_counter < (cycle_len + cycle_pos) - 1){
            orig = orig->next;
            hops_counter++;
            SLinkedListAddAtTail(copy, orig->val);
        }
        
        // creating cycle on copied list
       SLinkedListGetTail(copy)->next = SLinkedListGet(copy, cycle_pos);
    }
    else{
        while(orig->next != NULL){
            
            orig = orig->next; // since we've already copied the head at creation, we first advance the list 
            SLinkedListAddAtTail(copy, orig->val); // malloc a new element which takes the value of the current origin list,
                                                   // points head->next to the new elem and new_elem->next to NULL
        }
    }
    return copy;
}


#endif




