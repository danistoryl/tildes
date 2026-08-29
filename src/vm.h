#ifndef INTERPRETER_VM_H
#define INTERPRETER_VM_H

#include "types.h"
#include "value.h"
#include "object.h"
#include "ast.h"
#include <stddef.h>

// --- Interpretation Result ---
typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

// --- Bytecode Instructions ---
typedef enum {
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_POP,
    OP_LOAD_LOCAL,
    OP_STORE_LOCAL,
    OP_LOAD_GLOBAL,
    OP_STORE_GLOBAL,
    OP_LOAD_UPVALUE,
    OP_STORE_UPVALUE,
    OP_GET_PROPERTY,
    OP_SET_PROPERTY,
    OP_GET_SUBSCRIPT,
    OP_SET_SUBSCRIPT,
    
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULO,
    OP_NEGATE,
    OP_NOT,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_LOOP,
    OP_CALL,
    OP_RETURN,
    
    OP_BUILD_LIST,
    OP_BUILD_MAP,
    OP_BUILD_RECORD,
    
    OP_CAST,
    OP_AWAIT,
    OP_SPAWN_ASYNC
} OpCode;

// --- VM Configuration ---
#define VM_STACK_MAX 1024
#define VM_FRAMES_MAX 64

// --- Call Frame ---
typedef struct {
    ObjFunc* function;
    uint8_t* ip;
    Value* slots;
} CallFrame;

// --- VM State ---
typedef struct {
    GC gc;
    Value stack[VM_STACK_MAX];
    Value* stack_top;
    CallFrame frames[VM_FRAMES_MAX];
    int frame_count;
    ObjString* strings[1024];
    size_t string_count;
    Error error;
    bool is_running;
} VM;

// VM functions
void vm_init(VM* vm);
void vm_free(VM* vm);
InterpretResult vm_run(VM* vm, const char* source);
InterpretResult vm_execute(VM* vm, uint8_t* bytecode, size_t length);

void vm_push(VM* vm, Value value);
Value vm_pop(VM* vm);

#endif
