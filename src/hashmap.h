#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdbool.h>

#define HASHMAP_INITIAL_CAPACITY 16
#define HASHMAP_LOAD_FACTOR 0.75

typedef struct HashEntry {
    char* key;
    void* value;
    struct HashEntry* next;
} HashEntry;

typedef struct HashMap {
    HashEntry** buckets;
    size_t capacity;
    size_t size;
} HashMap;

HashMap* hashmap_create(void);
void hashmap_destroy(HashMap* map);
unsigned long hashmap_hash(const char* key);
bool hashmap_put(HashMap* map, const char* key, void* value);
void* hashmap_get(HashMap* map, const char* key);
bool hashmap_remove(HashMap* map, const char* key);
bool hashmap_contains(HashMap* map, const char* key);
size_t hashmap_size(HashMap* map);

#endif
