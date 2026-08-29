#include "object.h"
#include <stdlib.h>
#include <string.h>

uint32_t hash_string(const char* str, size_t len) {
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < len; i++) {
        hash ^= (uint8_t)str[i];
        hash *= 16777619u;
    }
    return hash;
}

ObjString* obj_new_string(GC* gc, const char* str, size_t len) {
    ObjString* s = (ObjString*)gc_alloc(gc, sizeof(ObjString));
    s->obj.type = OBJ_STRING;
    s->obj.is_marked = false;
    s->obj.next = NULL;
    s->length = len;
    s->chars = (char*)gc_alloc(gc, len + 1);
    memcpy(s->chars, str, len);
    s->chars[len] = '\0';
    s->hash = hash_string(str, len);
    return s;
}

ObjList* obj_new_list(GC* gc) {
    ObjList* list = (ObjList*)gc_alloc(gc, sizeof(ObjList));
    list->obj.type = OBJ_LIST;
    list->obj.is_marked = false;
    list->obj.next = NULL;
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
    return list;
}

ObjMap* obj_new_map(GC* gc) {
    ObjMap* map = (ObjMap*)gc_alloc(gc, sizeof(ObjMap));
    map->obj.type = OBJ_MAP;
    map->obj.is_marked = false;
    map->obj.next = NULL;
    map->entries = NULL;
    map->count = 0;
    map->capacity = 0;
    return map;
}

ObjFunc* obj_new_func(GC* gc, const char* name) {
    ObjFunc* func = (ObjFunc*)gc_alloc(gc, sizeof(ObjFunc));
    func->obj.type = OBJ_FUNC;
    func->obj.is_marked = false;
    func->obj.next = NULL;
    func->name = NULL;
    if (name) {
        size_t len = strlen(name);
        func->name = (char*)gc_alloc(gc, len + 1);
        memcpy(func->name, name, len);
        func->name[len] = '\0';
    }
    func->arity = 0;
    func->is_async = false;
    func->is_pub = false;
    func->bytecode = NULL;
    return func;
}
