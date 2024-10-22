/*

Using linked list: each element of the hash map is the head of a linked list, different keys can
hash to the same bucket. If they do, we can use a linked list to handle collision for instance, 
we'll find the linked listS array index and then iterate through the different keys inside the linked
list.
(I'll use my implementation of a singly linked list since it is the one I've developped the most)

*/


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash_map.h"

int main()
{
    #ifdef comment
    HMap hash_map;
    HMapConfig(&hash_map, 10, 100);
    HMapInit(&hash_map); // here we pass the hash table by ref to allocate "buckets", changing their reference
    if(hash_map.buckets == NULL)
        return 49;
    //SLinkedList* hash_map[HASH_MAP_MAX_SIZE];
    
    // creating n_buckets linked lists with 5 elements each with multiples of i (merely a simulation)
    for(int i = 0; i<hash_map.n_buckets; i++){
        hash_map.buckets[i] = NULL; // creating empty list
        Data kv_pair;
        for(int j = 0; j<5; j++){
            kv_pair.key = j+i;
            kv_pair.val = j*i;
            if(hash_map.buckets[i] == NULL){
                hash_map.buckets[i] = SLinkedListCreateNode(kv_pair);
            }
            else
                SLinkedListAddAtTail(hash_map.buckets[i], kv_pair);
        }
        printf("Linked list %d : \n", i);
        SLinkedListDump(hash_map.buckets[i]);
    }
    //SLinkedListDeleteAtIndex(hash_map.buckets[1], 2);
    //SLinkedListDeleteAtTail(hash_map.buckets[1]);
    //SLinkedListDump(hash_map.buckets[1]);
    
    /* Testing actual hash map functions */
    int searched_key = 1;
    SLinkedList* d1 = SLinkedListGetByKey(hash_map.buckets[1], searched_key);
    if(d1){
        printf("Value of key %d: %d \n", searched_key, d1->kv_pair.val);   
    }
    Data d2 = {54, 45};
    HMapAdd(&hash_map, d2);
    Data d3 = {84, 48};
    HMapAdd(&hash_map, d3);
    Data d4 = {4, 40};
    HMapAdd(&hash_map, d4);
    SLinkedListDump(hash_map.buckets[4]);
    SLinkedList* searched_node = HMapGet(&hash_map, d3.key);
    if(searched_node != NULL){
        printf("Searched node by key %d has value %d \n", searched_node->kv_pair.key, searched_node->kv_pair.val); 
    }
    #endif
    /* ------------------------- */
    
    HMap hash_map2;
    HMapConfig(&hash_map2, 10, 100);
    HMapInit(&hash_map2);
    
    srand(time(0));
    Data filler;
    for (int i = 0; i < 40; i++) {
        filler.key = i; // Generates a number between 0 and 100
        filler.val = filler.key*10;
        HMapAdd(&hash_map2, filler);
    }
    HMapDump(&hash_map2);
    HMapRemove(&hash_map2, 6);
    HMapDump(&hash_map2);
    HMapDelete(&hash_map2);
    
    printf("------------\n");
    //HMapConfig(&hash_map2, 10, 100);
    //HMapInit(&hash_map2);
    Data d2 = {54, 45};
    HMapAdd(&hash_map2, d2);
    HMapDump(&hash_map2);
    return 0;
}