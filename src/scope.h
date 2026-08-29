#ifndef SCOPE_H
#define SCOPE_H

#include "hashmap.h"
#include "value.h"
#include <stdbool.h>

typedef struct {
    Value value;
    bool is_mutable;
} VarInfo;

typedef struct Scope {
    HashMap* variables;
    struct Scope* parent;
    bool is_global;
} Scope;

Scope* scope_create(Scope* parent);
void scope_destroy(Scope* scope);
bool scope_define_var(Scope* scope, const char* name, Value value, bool is_mutable);
Value* scope_lookup(Scope* scope, const char* name);
bool scope_assign(Scope* scope, const char* name, Value value);
Scope* scope_get_global(Scope* scope);

#endif
