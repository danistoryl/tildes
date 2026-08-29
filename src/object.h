#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include "types.h"
#include "value.h"
#include <stddef.h>
#include <stdint.h>

// --- Object Types ---
typedef enum {
    OBJ_STRING,
    OBJ_LIST,
    OBJ_MAP,
    OBJ_FUNC,
    OBJ_STRUCT,
    OBJ_ENUM
} ObjType;

// --- Base Object ---
typedef struct Obj {
    ObjType type;
    bool is_marked;
    struct Obj* next;
} Obj;

// --- String Object ---
typedef struct ObjString {
    Obj obj;
    size_t length;
    char* chars;
    uint32_t hash;
} ObjString;

// --- List Object ---
typedef struct ObjList {
    Obj obj;
    Value* items;
    size_t count;
    size_t capacity;
} ObjList;

// --- Map Entry ---
typedef struct MapEntry {
    Value key;
    Value value;
    struct MapEntry* next;
} MapEntry;

// --- Map Object ---
typedef struct ObjMap {
    Obj obj;
    MapEntry** entries;
    size_t count;
    size_t capacity;
} ObjMap;

// --- Function Object ---
typedef struct ObjFunc {
    Obj obj;
    char* name;
    size_t arity;
    bool is_async;
    bool is_pub;
    void* bytecode;  // Will point to VM bytecode
} ObjFunc;

// Object creation functions
ObjString* obj_new_string(GC* gc, const char* str, size_t len);
ObjList* obj_new_list(GC* gc);
ObjMap* obj_new_map(GC* gc);
ObjFunc* obj_new_func(GC* gc, const char* name);

uint32_t hash_string(const char* str, size_t len);

#endif
