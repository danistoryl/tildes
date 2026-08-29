#include "value.h"
#include "object.h"
#include <stdio.h>
#include <string.h>

Value value_nil(void) {
    return NIL_VAL;
}

Value value_bool(bool b) {
    return BOOL_VAL(b);
}

Value value_i32(int32_t i) {
    return NUMBER_VAL((double)i);
}

Value value_f64(double d) {
    return NUMBER_VAL(d);
}

Value value_string(ObjString* s) {
    return STRING_VAL(s);
}

bool values_equal(Value a, Value b) {
    if (a.type != b.type) return false;
    
    switch (a.type) {
        case VAL_NIL: return true;
        case VAL_BOOL: return a.as_bool == b.as_bool;
        case VAL_NUMBER: return a.as_number == b.as_number;
        case VAL_STRING: return a.as_string == b.as_string;
        default: return false;
    }
}

void value_print(Value value) {
    switch (value.type) {
        case VAL_NIL: printf("nil"); break;
        case VAL_BOOL: printf("%s", value.as_bool ? "true" : "false"); break;
        case VAL_NUMBER: printf("%g", value.as_number); break;
        case VAL_STRING: 
            if (value.as_string) {
                printf("%s", value.as_string->chars);
            }
            break;
        case VAL_NATIVE: printf("<native fn>"); break;
        default: printf("<unknown>"); break;
    }
}
