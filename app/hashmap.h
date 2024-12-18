#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


#define MAX_ENTRY_STR_SIZE 128
#define MAX_MAP_SIZE 7199369


const static int Primes[] =
    {
        3, 7, 11, 17, 23, 29, 37, 47, 59, 71,
        89, 107, 131, 163, 197, 239, 293, 353,
        431, 521, 631, 761, 919, 1103, 1327, 1597,
        1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013,
        8419, 10103, 12143, 14591, 17519, 21023, 25229,
        30293, 36353, 43627, 52361, 62851, 75431, 90523,
        108631, 130363, 156437, 187751, 225307, 270371,
        324449, 389357, 467237, 560689, 672827, 807403,
        968897, 1162687, 1395263, 1674319, 2009191, 2411033,
        2893249, 3471899, 4166287, 4999559, 5999471, 7199369};


struct HashMap
{
    struct HashMapNode *nodes; //Store real values
    int *bucket; // Store bucket index, Bucket index is 1 based
    int size; //Current size of hash map
    int capacity; //MAX capacity
};

struct HashMapNode
{
    uint32_t hash;
    char *key;
    struct HashMapItem *item;
    int next; //Next index in the bucket
};


struct HashMapItem {
    char *val;
    long long ttl;
};


int find_nearest_prime(int num);
struct HashMapNode *hashmap_node_init();
struct HashMap *hashmap_init(int capacity);
void hashmap_insert(struct HashMap *h, struct HashMapItem *item);
struct HashMapItem *hashmap_get(struct HashMap *h,const char *key);



#endif 