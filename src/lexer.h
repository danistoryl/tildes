/*
 * lexer.h - Lexer interface
 * Tokenizes source code into tokens
 */

#ifndef LEXER_H
#define LEXER_H

#include "interpreter.h"

/* Lexer state */
typedef struct {
    const char *source;
    size_t length;
    size_t start;
    size_t current;
    size_t line;
    size_t column;
} Lexer;

/* Lexer functions */
Lexer lexer_init(const char *source);
Token lexer_next_token(Lexer *lexer);
void lexer_free(Lexer *lexer);

#endif /* LEXER_H */
