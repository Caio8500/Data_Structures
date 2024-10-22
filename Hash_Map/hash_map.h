#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "sLinkedList.h"

typedef struct HMap HMap;

typedef struct Data{
    int key; 
    void* val;
} Data;

// typedef SLinkedList** HMap; // the hash map is nothing more than an array of Linked Lists, since 
// we'll create them dinamically, we'll use a pointer of pointers, otherwise we could use
// SLinkedList* [HASH_MAP_MAX_BUCKETS] HMap; 

/* Macros for making data access easier syntax-wise */
#define GET_KEY(list_node) \
    ((Data*)list_node->data)->key

#define GET_VAL(list_node, data_type) \
    (*((data_type*)((Data*)list_node->data)->val))

HMap* HMapCreate(int hmap_size, size_t data_size);
bool HMapConfig(HMap* hash_map, int hmap_size, size_t data_size);
bool HMapInit(HMap* hash_map);
void HMapDelete(HMap** hash_map);
bool HMapAdd(HMap* hash_map, Data kv_pair);
bool HMapRemove(HMap* hash_map, int key);
SLinkedListNode* HMapGet(HMap* hash_map, int key);
SLinkedListNode* SLinkedListGetByKey(SLinkedList* list, int key);
SLinkedListNode* SLinkedListGetUpdateByKey(SLinkedList* list, Data kv_pair, size_t data_size);
bool SLinkedListDeleteByKey(SLinkedList* list, int key);
int HMapComputeHash(int key, int n_buckets);
void HMapDump(HMap* hash_map);
#endif