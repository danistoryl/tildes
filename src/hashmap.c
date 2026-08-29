#define _GNU_SOURCE
#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

unsigned long hashmap_hash(const char* key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

HashMap* hashmap_create(void) {
    HashMap* map = malloc(sizeof(HashMap));
    if (!map) return NULL;
    
    map->capacity = HASHMAP_INITIAL_CAPACITY;
    map->size = 0;
    map->buckets = calloc(map->capacity, sizeof(HashEntry*));
    if (!map->buckets) {
        free(map);
        return NULL;
    }
    return map;
}

void hashmap_destroy(HashMap* map) {
    if (!map) return;
    
    for (size_t i = 0; i < map->capacity; i++) {
        HashEntry* entry = map->buckets[i];
        while (entry) {
            HashEntry* next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    free(map->buckets);
    free(map);
}

bool hashmap_put(HashMap* map, const char* key, void* value) {
    if (!map || !key) return false;
    
    unsigned long index = hashmap_hash(key) % map->capacity;
    HashEntry* entry = map->buckets[index];
    
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return true;
        }
        entry = entry->next;
    }
    
    entry = malloc(sizeof(HashEntry));
    if (!entry) return false;
    
    entry->key = strdup(key);
    entry->value = value;
    entry->next = map->buckets[index];
    map->buckets[index] = entry;
    map->size++;
    
    return true;
}

void* hashmap_get(HashMap* map, const char* key) {
    if (!map || !key) return NULL;
    
    unsigned long index = hashmap_hash(key) % map->capacity;
    HashEntry* entry = map->buckets[index];
    
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

bool hashmap_remove(HashMap* map, const char* key) {
    if (!map || !key) return false;
    
    unsigned long index = hashmap_hash(key) % map->capacity;
    HashEntry* entry = map->buckets[index];
    HashEntry* prev = NULL;
    
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                map->buckets[index] = entry->next;
            }
            free(entry->key);
            free(entry);
            map->size--;
            return true;
        }
        prev = entry;
        entry = entry->next;
    }
    return false;
}

bool hashmap_contains(HashMap* map, const char* key) {
    return hashmap_get(map, key) != NULL;
}

size_t hashmap_size(HashMap* map) {
    return map ? map->size : 0;
}
