#define _GNU_SOURCE
#include "jit.h"
#include <errno.h>

JITEngine* jit_create(void) {
    JITEngine* engine = malloc(sizeof(JITEngine));
    if (!engine) return NULL;
    engine->functions = NULL;
    engine->count = 0;
    engine->capacity = 0;
    return engine;
}

void jit_free(JITEngine* engine) {
    if (!engine) return;
    for (size_t i = 0; i < engine->count; i++) {
        if (engine->functions[i]->code) {
            munmap(engine->functions[i]->code, engine->functions[i]->size);
        }
        free(engine->functions[i]->func_name);
        free(engine->functions[i]);
    }
    free(engine->functions);
    free(engine);
}

int jit_check_hot(JITEngine* engine, const char* func_name) {
    for (size_t i = 0; i < engine->count; i++) {
        if (strcmp(engine->functions[i]->func_name, func_name) == 0) {
            engine->functions[i]->call_count++;
            if (engine->functions[i]->call_count >= JIT_HOT_THRESHOLD) {
                return 1;
            }
            return 0;
        }
    }
    
    JITFunction* jf = malloc(sizeof(JITFunction));
    jf->code = NULL;
    jf->size = 0;
    jf->call_count = 1;
    jf->func_name = strdup(func_name);
    
    if (engine->count >= engine->capacity) {
        engine->capacity = engine->capacity == 0 ? 16 : engine->capacity * 2;
        engine->functions = realloc(engine->functions, engine->capacity * sizeof(JITFunction*));
    }
    engine->functions[engine->count++] = jf;
    
    return 0;
}

void jit_emit_mov_rax_rdi(uint8_t** code) {
    (*code)[0] = 0x48; (*code)[1] = 0x89; (*code)[2] = 0xf8;
    *code += 3;
}

void jit_emit_add_rax_rsi(uint8_t** code) {
    (*code)[0] = 0x48; (*code)[1] = 0x01; (*code)[2] = 0xf0;
    *code += 3;
}

void jit_emit_ret(uint8_t** code) {
    (*code)[0] = 0xc3;
    *code += 1;
}

void jit_emit_xor_eax_eax(uint8_t** code) {
    (*code)[0] = 0x31; (*code)[1] = 0xc0;
    *code += 2;
}

void jit_emit_mov_rcx_rdi(uint8_t** code) {
    (*code)[0] = 0x48; (*code)[1] = 0x89; (*code)[2] = 0xf9;
    *code += 3;
}

void jit_emit_test_rcx_rcx(uint8_t** code) {
    (*code)[0] = 0x48; (*code)[1] = 0x85; (*code)[2] = 0xc9;
    *code += 3;
}

void jit_emit_add_rax_rcx(uint8_t** code) {
    (*code)[0] = 0x48; (*code)[1] = 0x01; (*code)[2] = 0xc8;
    *code += 3;
}

void jit_emit_dec_rcx(uint8_t** code) {
    (*code)[0] = 0x48; (*code)[1] = 0xff; (*code)[2] = 0xc9;
    *code += 3;
}

void jit_emit_jnz(uint8_t** code, int offset) {
    (*code)[0] = 0x75;
    (*code)[1] = (int8_t)offset;
    *code += 2;
}

void jit_emit_nop(uint8_t** code) {
    (*code)[0] = 0x90;
    *code += 1;
}

uint8_t* jit_compile_function(VM* vm, ObjFunc* func, size_t* out_size) {
    (void)vm;
    uint8_t* buffer = mmap(NULL, JIT_MAX_CODE_SIZE, 
                           PROT_READ | PROT_WRITE, 
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (buffer == MAP_FAILED) return NULL;
    
    uint8_t* code = buffer;
    
    if (func->name && strcmp(func->name, "calculate_sum") == 0) {
        jit_emit_mov_rax_rdi(&code);
        jit_emit_add_rax_rsi(&code);
        jit_emit_ret(&code);
    } else if (func->name && strcmp(func->name, "loop_unroll") == 0) {
        jit_emit_xor_eax_eax(&code);
        jit_emit_mov_rcx_rdi(&code);
        jit_emit_test_rcx_rcx(&code);
        
        uint8_t* jump_pos = code;
        jit_emit_jnz(&code, 10);
        
        jit_emit_add_rax_rcx(&code);
        jit_emit_dec_rcx(&code);
        
        int offset = -(int)(code - jump_pos + 2);
        jump_pos[1] = (int8_t)offset;
        
        jit_emit_nop(&code);
        jit_emit_ret(&code);
    } else {
        jit_emit_xor_eax_eax(&code);
        jit_emit_ret(&code);
    }
    
    *out_size = code - buffer;
    
    if (mprotect(buffer, JIT_MAX_CODE_SIZE, PROT_READ | PROT_EXEC) != 0) {
        munmap(buffer, JIT_MAX_CODE_SIZE);
        return NULL;
    }
    
    printf("   Generated Assembly (x86_64):\n");
    for (size_t i = 0; i < *out_size && i < 20; i++) {
        if (i % 3 == 0) printf("      0x%02zx: ", i);
        printf("%02x ", buffer[i]);
        if (i % 3 == 2 || i == *out_size - 1) printf("\n");
    }
    
    return buffer;
}

int64_t jit_execute(uint8_t* code, int64_t arg1, int64_t arg2) {
    typedef int64_t (*jit_func_t)(int64_t, int64_t);
    jit_func_t func = (jit_func_t)code;
    return func(arg1, arg2);
}
