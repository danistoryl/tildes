#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "value.h"
#include "vm.h"

// Memory macros from VM
#define GROW_CAPACITY(cap) ((cap) == 0 ? 8 : (cap) * 2)
#define GROW_ARRAY(type, ptr, oldCap, newCap) \
    ((type*)realloc(ptr, sizeof(type) * (newCap)))
#define FREE_ARRAY(type, ptr, oldCap) free(ptr)

typedef struct {
    bool had_error;
    bool runtime_error;
    char message[256];
} ErrorReport;

static ErrorReport global_error = {false, false, ""};

static inline void error_runtime_local(ErrorReport* report, const char* format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "Runtime Error: ");
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    report->runtime_error = true;
}

typedef struct {
    uint8_t opcode;
    Value constant;
} ByteCodeInstruction;

typedef struct {
    ByteCodeInstruction* code;
    size_t count;
    size_t capacity;
} Chunk;

typedef struct {
    char* magic;
    uint32_t version;
    Chunk chunk;
} ByteCodeFile;

void chunk_init(Chunk* chunk) {
    chunk->code = NULL;
    chunk->count = 0;
    chunk->capacity = 0;
}

void chunk_write(Chunk* chunk, uint8_t opcode, Value constant) {
    if (chunk->capacity < chunk->count + 1) {
        size_t old_cap = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old_cap);
        chunk->code = GROW_ARRAY(ByteCodeInstruction, chunk->code, old_cap, chunk->capacity);
    }
    
    chunk->code[chunk->count].opcode = opcode;
    chunk->code[chunk->count].constant = constant;
    chunk->count++;
}

void chunk_free(Chunk* chunk) {
    FREE_ARRAY(ByteCodeInstruction, chunk->code, chunk->capacity);
    chunk_init(chunk);
}

// Simple compiler: converts AST to bytecode
// In a full implementation, this would walk the AST and emit opcodes
ByteCodeFile* compile_ast(ASTNode* ast) {
    if (!ast) return NULL;
    
    ByteCodeFile* bc = (ByteCodeFile*)malloc(sizeof(ByteCodeFile));
    bc->magic = "FBC1"; // FCL ByteCode version 1
    bc->version = 1;
    chunk_init(&bc->chunk);
    
    // Placeholder: Just emit a HALT for now
    // Real implementation would traverse AST nodes
    chunk_write(&bc->chunk, 0x01, NIL_VAL); // OP_HALT
    
    return bc;
}

int save_bytecode(ByteCodeFile* bc, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        error_runtime_local(&global_error, "Could not open file %s for writing", filename);
        return -1;
    }
    
    // Write header
    fwrite(bc->magic, 1, 4, file);
    fwrite(&bc->version, sizeof(uint32_t), 1, file);
    fwrite(&bc->chunk.count, sizeof(size_t), 1, file);
    
    // Write instructions
    for (size_t i = 0; i < bc->chunk.count; i++) {
        fwrite(&bc->chunk.code[i].opcode, sizeof(uint8_t), 1, file);
        // Simplified: we don't serialize complex values in this demo
        // A real impl would serialize the constant pool
    }
    
    fclose(file);
    return 0;
}

ByteCodeFile* load_bytecode(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        error_runtime_local(&global_error, "Could not open file %s for reading", filename);
        return NULL;
    }
    
    ByteCodeFile* bc = (ByteCodeFile*)malloc(sizeof(ByteCodeFile));
    
    // Read header
    bc->magic = (char*)malloc(5);
    fread(bc->magic, 1, 4, file);
    bc->magic[4] = '\0';
    
    if (strcmp(bc->magic, "FBC1") != 0) {
        error_runtime_local(&global_error, "Invalid bytecode file format");
        free(bc->magic);
        free(bc);
        fclose(file);
        return NULL;
    }
    
    fread(&bc->version, sizeof(uint32_t), 1, file);
    size_t count;
    fread(&count, sizeof(size_t), 1, file);
    
    chunk_init(&bc->chunk);
    bc->chunk.capacity = count;
    bc->chunk.code = (ByteCodeInstruction*)malloc(count * sizeof(ByteCodeInstruction));
    bc->chunk.count = count;
    
    // Read instructions
    for (size_t i = 0; i < count; i++) {
        fread(&bc->chunk.code[i].opcode, sizeof(uint8_t), 1, file);
        bc->chunk.code[i].constant = NIL_VAL; // Simplified
    }
    
    fclose(file);
    return bc;
}

void bytecode_free(ByteCodeFile* bc) {
    if (!bc) return;
    free(bc->magic);
    chunk_free(&bc->chunk);
    free(bc);
}
