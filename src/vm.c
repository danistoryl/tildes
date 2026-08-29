#include "vm.h"
#include "lexer.h"
#include "parser.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vm_init(VM* vm) {
    gc_init(&vm->gc);
    vm->stack_top = vm->stack;
    vm->frame_count = 0;
    vm->string_count = 0;
    vm->is_running = false;
    vm->error = error_new(ERR_NONE, 0, 0, NULL);
}

void vm_free(VM* vm) {
    (void)vm;
    // Free all objects
    // (Real implementation would traverse and free all GC objects)
}

void vm_push(VM* vm, Value value) {
    *vm->stack_top = value;
    vm->stack_top++;
}

Value vm_pop(VM* vm) {
    vm->stack_top--;
    return *vm->stack_top;
}

static InterpretResult vm_run_bytecode(VM* vm, uint8_t* bytecode, size_t length) {
    uint8_t* ip = bytecode;
    (void)length;
    
    #define READ_BYTE() (*ip++)
    
    for (;;) {
        uint8_t instruction = READ_BYTE();
        
        switch (instruction) {
            case OP_NIL:
                vm_push(vm, value_nil());
                break;
                
            case OP_TRUE:
                vm_push(vm, value_bool(true));
                break;
                
            case OP_FALSE:
                vm_push(vm, value_bool(false));
                break;
                
            case OP_POP:
                vm_pop(vm);
                break;
                
            case OP_RETURN:
                return INTERPRET_OK;
                
            default:
                return INTERPRET_RUNTIME_ERROR;
        }
    }
    
    #undef READ_BYTE
}

InterpretResult vm_run(VM* vm, const char* source) {
    vm->is_running = true;
    
    // Simple test - just tokenize and return
    Lexer lex = lexer_new(source);
    Token token;
    
    do {
        token = lexer_next(&lex);
        if (token.type == TOKEN_ERROR) {
            vm->is_running = false;
            return INTERPRET_COMPILE_ERROR;
        }
    } while (token.type != TOKEN_EOF);
    
    // For now, just return OK - full parser/VM coming next
    vm->is_running = false;
    return INTERPRET_OK;
}

InterpretResult vm_execute(VM* vm, uint8_t* bytecode, size_t length) {
    return vm_run_bytecode(vm, bytecode, length);
}
