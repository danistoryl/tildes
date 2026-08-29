#ifndef INTERPRETER_VALUE_H
#define INTERPRETER_VALUE_H

#include "types.h"
#include <stdint.h>
#include <stdbool.h>

// Forward declare Obj types
typedef struct ObjString ObjString;
typedef struct ObjList ObjList;
typedef struct ObjMap ObjMap;
typedef struct ObjFunction ObjFunction;

// Forward declare Value first
struct Value;

// Define NativeFn type
typedef struct Value (*NativeFn)(int arg_count, struct Value* args);

// Value type tags
typedef enum {
    VAL_NIL,
    VAL_BOOL,
    VAL_NUMBER,
    VAL_STRING,
    VAL_NATIVE,
    VAL_LIST,
    VAL_MAP
} ValueType;

// --- Value Union ---
typedef struct Value {
    ValueType type;
    union {
        bool as_bool;
        double as_number;
        ObjString* as_string;
        ObjList* as_list;
        ObjMap* as_map;
        NativeFn as_native;
        void* as_ptr;
    };
} Value;

// Macros for creating values
#define NIL_VAL ((Value){VAL_NIL, {.as_ptr = NULL}})
#define BOOL_VAL(b) ((Value){VAL_BOOL, {.as_bool = (b)}})
#define NUMBER_VAL(n) ((Value){VAL_NUMBER, {.as_number = (n)}})
#define STRING_VAL(s) ((Value){VAL_STRING, {.as_string = (ObjString*)(s)}})
#define NATIVE_VAL(fn) ((Value){VAL_NATIVE, {.as_native = (fn)}})

// Macros for checking types
#define IS_NIL(v) ((v).type == VAL_NIL)
#define IS_BOOL(v) ((v).type == VAL_BOOL)
#define IS_NUMBER(v) ((v).type == VAL_NUMBER)
#define IS_STRING(v) ((v).type == VAL_STRING)
#define IS_NATIVE(v) ((v).type == VAL_NATIVE)

// Macros for extracting values
#define AS_BOOL(v) ((v).as_bool)
#define AS_NUMBER(v) ((v).as_number)
#define AS_STRING(v) ((v).as_string)
#define AS_NATIVE(v) ((v).as_native)

// Value functions
Value value_nil(void);
Value value_bool(bool b);
Value value_i32(int32_t i);
Value value_f64(double d);
Value value_string(ObjString* s);
bool values_equal(Value a, Value b);
void value_print(Value value);

#endif
