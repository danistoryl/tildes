#ifndef BUILTIN_H
#define BUILTIN_H

#include "vm.h"
#include "scope.h"

void builtin_init(VM* vm, Scope* global_scope);

#endif
