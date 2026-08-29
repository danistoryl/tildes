/*
 * main.c - Entry point for the interpreter
 * Minimal main function
 */

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "interpreter.h"

static void run_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        exit(1);
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *source = malloc(size + 1);
    if (source == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        exit(1);
    }
    
    fread(source, 1, size, file);
    source[size] = '\0';
    fclose(file);
    
    Lexer lexer = lexer_init(source);
    
    printf("Tokens from '%s':\n", filename);
    Token token;
    do {
        token = lexer_next_token(&lexer);
        printf("[%zu:%zu] %s", 
               token.line, 
               token.column,
               token_type_name(token.type));
        if (token.value != NULL) {
            printf(": '%s'", token.value);
        }
        printf("\n");
        token_free(&token);
    } while (token.type != TOKEN_EOF);
    
    lexer_free(&lexer);
    free(source);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    
    run_file(argv[1]);
    
    return 0;
}
