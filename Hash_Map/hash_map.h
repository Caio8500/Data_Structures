#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "sLinkedList.h"

#define HASH_MAP_MAX_SIZE 5

typedef struct HMap{
    SLinkedList** buckets; // array of linked lists
    int n_buckets; 
    int key_range;
} HMap;


// typedef SLinkedList** HMap; // the hash map is nothing more than an array of Linked Lists, since 
// we'll create them dinamically, we'll use a pointer of pointers, otherwise we could use
// SLinkedList* [HASH_MAP_MAX_BUCKETS] HMap; 

bool HMapConfig(HMap* hash_map, int map, int key_range);
bool HMapInit(HMap* hash_map);
void HMapDelete(HMap* hash_map);
bool HMapAdd(HMap* hash_map, Data kv_pair);
bool HMapRemove(HMap* hash_map, int key);
SLinkedList* HMapGet(HMap* hash_map, int key);
int HMapComputeHash(int key, int key_range, int n_buckets);
void HMapDump(HMap* hash_map);
#endif