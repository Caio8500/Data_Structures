#include <stdlib.h>
#include <stdio.h>
#include "hash_map.h"

bool HMapConfig(HMap* hash_map, int hmap_size, int key_range){
    if(hmap_size == 0 || key_range == 0 || hash_map == NULL){
        return false;
    }
    else
        hash_map->n_buckets = hmap_size;
        hash_map->key_range = key_range;
        return true;
}

bool HMapInit(HMap* hash_map){
    if(hash_map->n_buckets == 0 || hash_map->key_range == 0 || hash_map == NULL){
        printf("The Hash Map hasn't been configured before initialization! \n");
        return false;
    }
    else{
        hash_map->buckets = calloc(hash_map->n_buckets, sizeof(SLinkedList*)); // allocating multiple linked list pointers and initializaing them to NULL
        return true;
        
    }
}

void HMapDelete(HMap* hash_map){
    for(int i = 0; i< hash_map->n_buckets; i++){
        SLinkedListFree(&(hash_map->buckets[i]));
    }
    hash_map->n_buckets = 0;
    hash_map->key_range = 0;
    free(hash_map->buckets);
}

bool HMapAdd(HMap* hash_map, Data kv_pair){
    if(hash_map->n_buckets == 0 || hash_map->key_range == 0 || hash_map == NULL){
        printf("The Hash Map hasn't been configured before insertion! \n");
        return 1;
    }
    int bucket_index = HMapComputeHash(kv_pair.key, hash_map->key_range, hash_map->n_buckets);
    if(hash_map->buckets[bucket_index] == NULL){
        // creating head of list
        hash_map->buckets[bucket_index] = SLinkedListCreateNode(kv_pair);
        // equivalent to: SLinkedListAddAtHead(&(hash_map->buckets[bucket_index]), kv_pair);
        return 0;
    }
    else if(SLinkedListGetUpdateByKey(hash_map->buckets[bucket_index], kv_pair) == NULL){
        // this key doesn't exist yet in this bucket
        SLinkedListAddAtTail(hash_map->buckets[bucket_index], kv_pair);
        return 0;
    }
    return 1;
}

bool HMapRemove(HMap* hash_map, int key){
    int bucket_index = HMapComputeHash(key, hash_map->key_range, hash_map->n_buckets);
    if(SLinkedListDeleteByKey(&(hash_map->buckets[bucket_index]), key) == 0){
        return 0;
    }
    else{
        return 1; // key not present
    }
}

SLinkedList* HMapGet(HMap* hash_map, int key){
    int bucket_index = HMapComputeHash(key, hash_map->key_range, hash_map->n_buckets);
    SLinkedList* searched_node = SLinkedListGetByKey(hash_map->buckets[bucket_index], key);
    if(searched_node == NULL){
        return NULL;
    }
    else{
        return searched_node;
    }
}


int HMapComputeHash(int key, int key_range, int n_buckets){
    return key%(key_range/n_buckets);
}

void HMapDump(HMap* hash_map){
    for(int i = 0; i<hash_map->n_buckets; i++){
        printf("Bucket %d -> ", i);
        SLinkedListDump(hash_map->buckets[i]);
    }
}