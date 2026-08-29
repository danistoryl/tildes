#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "types.h"
#include "token.h"
#include "value.h"
#include "object.h"
#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "vm.h"
#include "error.h"
#include "cli.h"

// Main interpreter function
InterpretResult interpret(const char* source);

#endif
