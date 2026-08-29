#define _GNU_SOURCE
#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static Value builtin_print(int arg_count, Value* args) {
    for (int i = 0; i < arg_count; i++) {
        value_print(args[i]);
        if (i < arg_count - 1) printf(" ");
    }
    printf("\n");
    return NIL_VAL;
}

static Value builtin_input(int arg_count, Value* args) {
    (void)arg_count;
    (void)args;
    
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        return STRING_VAL(strdup(buffer));
    }
    return NIL_VAL;
}

static Value builtin_len(int arg_count, Value* args) {
    if (arg_count != 1) {
        fprintf(stderr, "Error: len() expects 1 argument\n");
        return NIL_VAL;
    }
    
    if (IS_STRING(args[0])) {
        return NUMBER_VAL((double)strlen(AS_STRING(args[0])->chars));
    }
    return NIL_VAL;
}

static Value builtin_type_of(int arg_count, Value* args) {
    if (arg_count != 1) {
        return STRING_VAL(strdup("error"));
    }
    
    const char* type_name;
    switch (args[0].type) {
        case VAL_BOOL: type_name = "bool"; break;
        case VAL_NUMBER: type_name = "num"; break;
        case VAL_STRING: type_name = "str"; break;
        case VAL_NIL: type_name = "nil"; break;
        case VAL_NATIVE: type_name = "native"; break;
        default: type_name = "unknown"; break;
    }
    return STRING_VAL(strdup(type_name));
}

static Value builtin_int(int arg_count, Value* args) {
    if (arg_count != 1) return NIL_VAL;
    
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL((double)(long)AS_NUMBER(args[0]));
    }
    if (IS_STRING(args[0])) {
        return NUMBER_VAL((double)atoi(AS_STRING(args[0])->chars));
    }
    return NIL_VAL;
}

static Value builtin_float_fn(int arg_count, Value* args) {
    if (arg_count != 1) return NIL_VAL;
    
    if (IS_NUMBER(args[0])) {
        return args[0];
    }
    if (IS_STRING(args[0])) {
        return NUMBER_VAL(atof(AS_STRING(args[0])->chars));
    }
    return NIL_VAL;
}

static Value builtin_string_fn(int arg_count, Value* args) {
    if (arg_count != 1) return NIL_VAL;
    
    char buffer[64];
    if (IS_NUMBER(args[0])) {
        snprintf(buffer, sizeof(buffer), "%g", AS_NUMBER(args[0]));
        return STRING_VAL(strdup(buffer));
    }
    if (IS_BOOL(args[0])) {
        return STRING_VAL(strdup(AS_BOOL(args[0]) ? "true" : "false"));
    }
    if (IS_STRING(args[0])) {
        return args[0];
    }
    return NIL_VAL;
}

static Value builtin_bool_fn(int arg_count, Value* args) {
    if (arg_count != 1) return NIL_VAL;
    
    if (IS_BOOL(args[0])) return args[0];
    if (IS_NUMBER(args[0])) {
        return BOOL_VAL(AS_NUMBER(args[0]) != 0);
    }
    if (IS_STRING(args[0])) {
        return BOOL_VAL(strlen(AS_STRING(args[0])->chars) > 0);
    }
    return BOOL_VAL(false);
}

static Value builtin_abs(int arg_count, Value* args) {
    if (arg_count != 1 || !IS_NUMBER(args[0])) return NIL_VAL;
    return NUMBER_VAL(fabs(AS_NUMBER(args[0])));
}

static Value builtin_sqrt(int arg_count, Value* args) {
    if (arg_count != 1 || !IS_NUMBER(args[0])) return NIL_VAL;
    return NUMBER_VAL(sqrt(AS_NUMBER(args[0])));
}

static Value builtin_pow(int arg_count, Value* args) {
    if (arg_count != 2 || !IS_NUMBER(args[0]) || !IS_NUMBER(args[1])) return NIL_VAL;
    return NUMBER_VAL(pow(AS_NUMBER(args[0]), AS_NUMBER(args[1])));
}

static Value builtin_min(int arg_count, Value* args) {
    if (arg_count < 2) return NIL_VAL;
    
    double min_val = AS_NUMBER(args[0]);
    for (int i = 1; i < arg_count; i++) {
        if (IS_NUMBER(args[i])) {
            double val = AS_NUMBER(args[i]);
            if (val < min_val) min_val = val;
        }
    }
    return NUMBER_VAL(min_val);
}

static Value builtin_max(int arg_count, Value* args) {
    if (arg_count < 2) return NIL_VAL;
    
    double max_val = AS_NUMBER(args[0]);
    for (int i = 1; i < arg_count; i++) {
        if (IS_NUMBER(args[i])) {
            double val = AS_NUMBER(args[i]);
            if (val > max_val) max_val = val;
        }
    }
    return NUMBER_VAL(max_val);
}

void builtin_init(VM* vm, Scope* global_scope) {
    scope_define_var(global_scope, "print", NATIVE_VAL(builtin_print), false);
    scope_define_var(global_scope, "input", NATIVE_VAL(builtin_input), false);
    scope_define_var(global_scope, "len", NATIVE_VAL(builtin_len), false);
    scope_define_var(global_scope, "type_of", NATIVE_VAL(builtin_type_of), false);
    scope_define_var(global_scope, "int", NATIVE_VAL(builtin_int), false);
    scope_define_var(global_scope, "float", NATIVE_VAL(builtin_float_fn), false);
    scope_define_var(global_scope, "string", NATIVE_VAL(builtin_string_fn), false);
    scope_define_var(global_scope, "bool", NATIVE_VAL(builtin_bool_fn), false);
    scope_define_var(global_scope, "abs", NATIVE_VAL(builtin_abs), false);
    scope_define_var(global_scope, "sqrt", NATIVE_VAL(builtin_sqrt), false);
    scope_define_var(global_scope, "pow", NATIVE_VAL(builtin_pow), false);
    scope_define_var(global_scope, "min", NATIVE_VAL(builtin_min), false);
    scope_define_var(global_scope, "max", NATIVE_VAL(builtin_max), false);
    (void)vm;
}
