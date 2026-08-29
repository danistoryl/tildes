#include "types.h"
#include "object.h"
#include <stdlib.h>
#include <string.h>

#define GC_HEAP_GROW_FACTOR 2

void gc_init(GC* gc) {
    gc->roots = NULL;
    gc->total_bytes = 0;
    gc->next_gc = 1024 * 1024;
}

void* gc_alloc(GC* gc, size_t size) {
    gc->total_bytes += size;
    
    if (gc->total_bytes > gc->next_gc) {
        gc_collect(gc);
    }
    
    void* ptr = malloc(size);
    memset(ptr, 0, size);
    return ptr;
}

void gc_add_root(GC* gc, Obj* obj) {
    GcRoot* root = (GcRoot*)malloc(sizeof(GcRoot));
    root->obj = obj;
    root->next = gc->roots;
    gc->roots = root;
}

static void mark_object(Obj* obj) {
    if (obj == NULL) return;
    if (obj->is_marked) return;
    
    obj->is_marked = true;
    
    switch (obj->type) {
        case OBJ_STRING:
            break;
        case OBJ_LIST: {
            ObjList* list = (ObjList*)obj;
            for (size_t i = 0; i < list->count; i++) {
                // Mark values in list
            }
            break;
        }
        case OBJ_MAP: {
            ObjMap* map = (ObjMap*)obj;
            for (size_t i = 0; i < map->capacity; i++) {
                MapEntry* entry = map->entries[i];
                while (entry != NULL) {
                    // Mark key and value
                    entry = entry->next;
                }
            }
            break;
        }
        case OBJ_FUNC:
            break;
        default:
            break;
    }
}

void gc_collect(GC* gc) {
    // Mark phase
    GcRoot* root = gc->roots;
    while (root != NULL) {
        mark_object(root->obj);
        root = root->next;
    }
    
    // Sweep phase
    // (Simplified - real impl would traverse all objects)
    gc->next_gc = gc->total_bytes * GC_HEAP_GROW_FACTOR;
}
