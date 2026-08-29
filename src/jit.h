#ifndef FCL_JIT_H
#define FCL_JIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include "vm.h"
#include "object.h"

#define JIT_HOT_THRESHOLD 10
#define JIT_MAX_CODE_SIZE 4096

typedef struct {
    uint8_t* code;
    size_t size;
    int call_count;
    char* func_name;
} JITFunction;

typedef struct {
    JITFunction** functions;
    size_t count;
    size_t capacity;
} JITEngine;

JITEngine* jit_create(void);
void jit_free(JITEngine* engine);
int jit_check_hot(JITEngine* engine, const char* func_name);
uint8_t* jit_compile_function(VM* vm, ObjFunc* func, size_t* out_size);
void jit_emit_mov_rax_rdi(uint8_t** code);
void jit_emit_add_rax_rsi(uint8_t** code);
void jit_emit_ret(uint8_t** code);
void jit_emit_xor_eax_eax(uint8_t** code);
void jit_emit_dec_rcx(uint8_t** code);
void jit_emit_jnz(uint8_t** code, int offset);
int64_t jit_execute(uint8_t* code, int64_t arg1, int64_t arg2);

#endif
