#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash_map.h"

typedef struct HMap{
    SLinkedList** buckets; // array of linked lists
    int n_buckets; 
    size_t data_size;
} HMap;

HMap* HMapCreate(int hmap_size, size_t data_size){
    HMap* new_map = malloc(sizeof(HMap));
    HMapConfig(new_map, hmap_size, data_size);
    HMapInit(new_map);
    return new_map;
}

bool HMapConfig(HMap* hash_map, int hmap_size, size_t data_size){
    if(hmap_size == 0 || hash_map == NULL){
        return false;
    }
    else
        hash_map->n_buckets = hmap_size;
        hash_map->data_size = data_size;
        return true;
}

bool HMapInit(HMap* hash_map){
    if(hash_map->n_buckets == 0 || hash_map == NULL){
        printf("The Hash Map hasn't been configured before initialization! \n");
        return false;
    }
    else{
        hash_map->buckets = calloc(hash_map->n_buckets, sizeof(SLinkedList*)); // allocating multiple linked list pointers and initializaing them to NULL
        for(int i = 0; i<=hash_map->n_buckets; i++){
            hash_map->buckets[i] = SLinkedListCreate(hash_map->n_buckets, sizeof(Data));
        }
        return true;
        
    }
}

void HMapDelete(HMap** hash_map){
    for(int i = 0; i< (*hash_map)->n_buckets; i++){
        SLinkedListDelete((*hash_map)->buckets[i]);
    }
    (*hash_map)->n_buckets = 0;
    free((*hash_map)->buckets);
    free((*hash_map));
    *hash_map = NULL;
}

bool HMapAdd(HMap* hash_map, Data kv_pair){
    if(hash_map->n_buckets == 0 || hash_map == NULL){
        printf("The Hash Map hasn't been configured before insertion! \n");
        return 1;
    }
    int bucket_index = HMapComputeHash(kv_pair.key, hash_map->n_buckets);
    if(hash_map->buckets[bucket_index]->head == NULL){
        // creating head of list
        SLinkedListAddAtHead(hash_map->buckets[bucket_index], (void*)&kv_pair);
        // equivalent to: SLinkedListAddAtHead(&(hash_map->buckets[bucket_index]), kv_pair);
        return 0;
    }
    else if(SLinkedListGetUpdateByKey(hash_map->buckets[bucket_index], kv_pair, hash_map->data_size) == NULL){
        // this key doesn't exist yet in this bucket
        SLinkedListAddAtTail(hash_map->buckets[bucket_index], (void*)&kv_pair);
        return 0;
    }
    return 1;
}

int HMapComputeHash(int key, int n_buckets){
    return key%n_buckets;
}

void HMapDump(HMap* hash_map){
    for(int i = 0; i<hash_map->n_buckets; i++){
        printf("Bucket %d : \n", i);
        int hops_counter = 0;
        hash_map->buckets[i]->head == NULL;
        if(hash_map->buckets[i]->head != NULL){
            LINKED_LIST_FOR_EACH(hash_map->buckets[i], hash_map->buckets[i]->head){
            printf("(%d) -> addr: %p; key: %d; val: %.2f; next: %p\n", hops_counter, iterator, GET_KEY(iterator), GET_VAL(iterator, float), iterator->next);
            hops_counter++;
        }
        }
    }
}

bool HMapRemove(HMap* hash_map, int key){
    int bucket_index = HMapComputeHash(key, hash_map->n_buckets);
    if(SLinkedListDeleteByKey(hash_map->buckets[bucket_index], key) == 0){
        return 0;
    }
    else{
        return 1; // key not present
    }
}

SLinkedListNode* HMapGet(HMap* hash_map, int key){
    int bucket_index = HMapComputeHash(key, hash_map->n_buckets);
    SLinkedListNode* searched_node = SLinkedListGetByKey(hash_map->buckets[bucket_index], key);
    if(searched_node == NULL){
        return NULL;
    }
    else{
        return searched_node;
    }
}

bool SLinkedListDeleteByKey(SLinkedList* list, int key){
    // add support to head deletion
    if(list == NULL)
        return 1;
    SLinkedListNode* current_node = list->head; // careful not to change the head as we pass the head pointer's address
    int aux_idx = 0;
    
    while(current_node != NULL){
        if(current_node->next != NULL && ((Data *)(current_node->next->data))->key == key){
            SLinkedListNode* to_be_deleted = current_node->next;
            current_node->next  = current_node->next->next;
            // freeing up memory allocated by deleted node
            free(to_be_deleted->data);
            free(to_be_deleted);
            return true;
        }
        if(aux_idx == 0){
            // edge case for list head
            if(((Data *)(current_node->data))->key == key){
                SLinkedListDeleteAtHead(list);
                return true;
            }
        }
        current_node = current_node->next;
        aux_idx++;
    }
    return false;
}

SLinkedListNode* SLinkedListGetUpdateByKey(SLinkedList* list, Data kv_pair, size_t data_size){
    SLinkedListNode* current_node = list->head; 
    while(current_node != NULL){
        if(((Data *)(current_node->data))->key == kv_pair.key){
            memcpy(((Data *)(current_node->data))->val, kv_pair.val, data_size);
            //printf("*** %s: Updating key %d with value %d... ***\n", __func__, kv_pair.key, kv_pair.val);
            return current_node;
        }
        current_node = current_node->next;
    }
    return NULL;
}

SLinkedListNode* SLinkedListGetByKey(SLinkedList* list, int key){
    SLinkedListNode* current_node = list->head; 
    while(current_node != NULL){
        // we have to go all the way to the tail
        if(((Data *)(current_node->data))->key == key){
            return current_node;
        }
        current_node = current_node->next;
    }
    printf("*** %s: key %d is unavailable! ***\n", __func__, key);
    return NULL;
}
