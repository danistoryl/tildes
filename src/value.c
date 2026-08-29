#include "value.h"
#include <string.h>

Value value_nil(void) {
    Value v;
    v.type = TYPE_NIL;
    v.as_ptr = NULL;
    return v;
}

Value value_bool(bool b) {
    Value v;
    v.type = TYPE_BOOL;
    v.as_bool = b;
    return v;
}

Value value_i32(int32_t i) {
    Value v;
    v.type = TYPE_I32;
    v.as_i32 = i;
    return v;
}

Value value_f64(double d) {
    Value v;
    v.type = TYPE_F64;
    v.as_f64 = d;
    return v;
}

Value value_string(ObjString* s) {
    Value v;
    v.type = TYPE_STR;
    v.as_string = s;
    return v;
}

bool values_equal(Value a, Value b) {
    if (a.type != b.type) return false;
    
    switch (a.type) {
        case TYPE_NIL: return true;
        case TYPE_BOOL: return a.as_bool == b.as_bool;
        case TYPE_I32: return a.as_i32 == b.as_i32;
        case TYPE_F64: return a.as_f64 == b.as_f64;
        case TYPE_STR: return a.as_string == b.as_string;
        default: return false;
    }
}
