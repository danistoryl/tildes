#ifndef INTERPRETER_VALUE_H
#define INTERPRETER_VALUE_H

#include "types.h"
#include <stdint.h>
#include <stdbool.h>

// Forward declare Obj types
typedef struct ObjString ObjString;
typedef struct ObjList ObjList;
typedef struct ObjMap ObjMap;

// --- Value Union ---
typedef struct {
    TypeKind type;
    union {
        bool as_bool;
        int8_t as_i8;
        int16_t as_i16;
        int32_t as_i32;
        int64_t as_i64;
        float as_f8;
        double as_f16;
        float as_f32;
        double as_f64;
        ObjString* as_string;
        ObjList* as_list;
        ObjMap* as_map;
        void* as_ptr;
    };
} Value;

// Value functions
Value value_nil(void);
Value value_bool(bool b);
Value value_i32(int32_t i);
Value value_f64(double d);
Value value_string(ObjString* s);
bool values_equal(Value a, Value b);

#endif
