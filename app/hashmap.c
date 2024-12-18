//
// Created by amirreza on 12/16/24.
//

#include "hashmap.h"
#include "murmur.h"


int find_nearest_prime(int num)
{
    int low = 0, high = sizeof(Primes) - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (Primes[mid] < num)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return Primes[high];
}



struct HashMapNode *hashmap_node_init()
{
    struct HashMapNode *hNode =
        (struct HashMapNode *)malloc(sizeof(struct HashMapNode));
    return hNode;
}

struct HashMap *hashmap_init(int capacity)
{
    struct HashMap *h = (struct HashMap *)malloc(sizeof(struct HashMap));
    int nearest_prime = find_nearest_prime(capacity);
    h->nodes = malloc(nearest_prime * sizeof(struct HashMapNode*));
    h->bucket = malloc(nearest_prime * sizeof(int));
    memset(h->bucket, 0, nearest_prime * sizeof(int));
    for (int i = 0; i < nearest_prime; i++)
    {
        h->nodes[i].key = malloc(MAX_ENTRY_STR_SIZE);
        h->nodes[i].item = malloc(sizeof(struct HashMapItem));
        h->nodes[i].next = -1;
    }

    h->size = 0;
    h->capacity = nearest_prime;
    return h;
}


void resize(struct HashMap *h, uint32_t capacity)
{
    
}

void hashmap_insert(struct HashMap *h, const char *key, struct HashMapItem *item)
{
    uint32_t hash = murmur_hash_32(key, strlen(key), MURMUR_DEFAULT_SEED);
    int bucket_index = hash % h->capacity;
    int bucket = h->bucket[bucket_index];
    int i = bucket_index - 1;

    int collisions = 0;
    while(i < h->size && i >= 0)
    {
        printf("i: %d\n", i);
        if(h->nodes[i].hash == hash && strcmp(h->nodes[i].key, key) == 0)
        {
            free(h->nodes[i].item->val);
            h->nodes->item->val = strdup(item->val);
            h->nodes->item->ttl = item->ttl;
            return;
        }
        i = h->nodes[i].next;
        collisions++;
        if(collisions > h->size){
            perror("A concurrent insertion has happend");
            return;
        }
    }



    int index = h->size;
    h->size++;

    if(h->size == h->capacity)
    {
        printf("hashamp capacity reached");
        return;
    }

    struct HashMapNode *node = malloc(sizeof(struct HashMapNode)); 
    node->item = malloc(sizeof(struct HashMapItem));
    node->hash = hash;
    node->key = strdup(key);
    node->item->ttl = strdup(item->val);
    node->item->val = item->ttl;
    free(&(h->nodes[index]));
    h->nodes[index] = *node;
    h->nodes->next = bucket - 1;
    h->bucket[bucket_index] = index + 1;
}

struct HashMapItem *hashmap_get(struct HashMap *h,const char *key)
{
    if (h->size == 0)
        return NULL;

    uint32_t hash = murmur_hash_32(key, strlen(key), MURMUR_DEFAULT_SEED);
    int bucket_index = hash % h->capacity;
    int bucket = h->bucket[bucket_index];
    int collisions = 0, i = bucket - 1;
    do {
        if (i < 0 || i > h->capacity)
            return NULL;

        if (hash == h->nodes[i].hash && strcmp(key, h->nodes[i].key) == 0)
        {
            return h->nodes[i].item;
        }
        i = h->nodes[i].next;
        collisions++;

    } while (collisions <= h->capacity);

    perror("Concurrent update has happend");
    return NULL;
}

void hashmap_remove(struct HashMap *h, const char *key)
{
    
}