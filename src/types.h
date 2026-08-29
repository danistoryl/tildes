#ifndef INTERPRETER_TYPES_H
#define INTERPRETER_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// --- Data Types (CTypes matching) ---
typedef enum {
    TYPE_NIL,
    TYPE_BOOL,
    TYPE_I8, TYPE_I16, TYPE_I32, TYPE_I64,
    TYPE_F8, TYPE_F16, TYPE_F32, TYPE_F64,
    TYPE_STR,
    TYPE_LIST,
    TYPE_MAP,
    TYPE_SET,
    TYPE_RECORD,
    TYPE_ENUM,
    TYPE_STRUCT,
    TYPE_FUNC,
    TYPE_ASYNC,
    TYPE_VOID,
    TYPE_DYN
} TypeKind;

// Forward declarations for objects
typedef struct Obj Obj;
typedef struct ObjString ObjString;
typedef struct ObjList ObjList;
typedef struct ObjMap ObjMap;

// --- Error Handling (Modern Style) ---
typedef enum {
    ERR_NONE,
    ERR_LEXER,
    ERR_PARSER,
    ERR_RUNTIME,
    ERR_TYPE,
    ERR_IO
} ErrorKind;

typedef struct {
    ErrorKind kind;
    int line;
    int column;
    const char* message;
    bool is_fatal;
} Error;

static inline Error error_new(ErrorKind kind, int line, int col, const char* msg) {
    Error e = { .kind = kind, .line = line, .column = col, .message = msg, .is_fatal = false };
    return e;
}

static inline Error error_fatal(ErrorKind kind, int line, int col, const char* msg) {
    Error e = error_new(kind, line, col, msg);
    e.is_fatal = true;
    return e;
}

// --- Garbage Collection Root ---
typedef struct GcRoot {
    Obj* obj;
    struct GcRoot* next;
} GcRoot;

typedef struct {
    GcRoot* roots;
    size_t total_bytes;
    size_t next_gc;
} GC;

void gc_init(GC* gc);
void* gc_alloc(GC* gc, size_t size);
void gc_collect(GC* gc);

#endif
