/*
 * value.c - Runtime value management
 * Clean value creation and cleanup
 */

#define _POSIX_C_SOURCE 200809L

#include "interpreter.h"
#include <stdlib.h>
#include <string.h>

Value value_number(double num) {
    Value val;
    val.type = VALUE_NUMBER;
    val.data.number = num;
    return val;
}

Value value_string(const char *str) {
    Value val;
    val.type = VALUE_STRING;
    if (str != NULL) {
        val.data.string = strdup(str);
    } else {
        val.data.string = NULL;
    }
    return val;
}

Value value_bool(bool b) {
    Value val;
    val.type = VALUE_BOOL;
    val.data.boolean = b;
    return val;
}

Value value_null(void) {
    Value val;
    val.type = VALUE_NULL;
    return val;
}

void value_free(Value *val) {
    if (val == NULL) {
        return;
    }
    
    if (val->type == VALUE_STRING) {
        free(val->data.string);
        val->data.string = NULL;
    }
}
