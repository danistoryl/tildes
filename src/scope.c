#include "scope.h"
#include <stdlib.h>
#include <string.h>

Scope* scope_create(Scope* parent) {
    Scope* scope = malloc(sizeof(Scope));
    if (!scope) return NULL;
    
    scope->variables = hashmap_create();
    if (!scope->variables) {
        free(scope);
        return NULL;
    }
    
    scope->parent = parent;
    scope->is_global = (parent == NULL);
    return scope;
}

void scope_destroy(Scope* scope) {
    if (!scope) return;
    
    // Free all VarInfo structs
    for (size_t i = 0; i < scope->variables->capacity; i++) {
        HashEntry* entry = scope->variables->buckets[i];
        while (entry) {
            free(entry->value);  // Free VarInfo
            entry = entry->next;
        }
    }
    
    hashmap_destroy(scope->variables);
    free(scope);
}

bool scope_define_var(Scope* scope, const char* name, Value value, bool is_mutable) {
    if (!scope || !name) return false;
    
    VarInfo* info = malloc(sizeof(VarInfo));
    if (!info) return false;
    
    info->value = value;
    info->is_mutable = is_mutable;
    
    return hashmap_put(scope->variables, name, info);
}

Value* scope_lookup(Scope* scope, const char* name) {
    Scope* current = scope;
    
    while (current) {
        VarInfo* info = hashmap_get(current->variables, name);
        if (info) {
            return &info->value;
        }
        current = current->parent;
    }
    return NULL;
}

bool scope_assign(Scope* scope, const char* name, Value value) {
    Scope* current = scope;
    
    while (current) {
        VarInfo* info = hashmap_get(current->variables, name);
        if (info) {
            if (!info->is_mutable && !current->is_global) {
                return false;
            }
            info->value = value;
            return true;
        }
        current = current->parent;
    }
    return false;
}

Scope* scope_get_global(Scope* scope) {
    while (scope && scope->parent) {
        scope = scope->parent;
    }
    return scope;
}
