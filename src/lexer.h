#ifndef INTERPRETER_LEXER_H
#define INTERPRETER_LEXER_H

#include "token.h"
#include <stddef.h>

// --- Lexer State ---
typedef struct {
    const char* start;
    const char* current;
    int line;
    int column;
    Error error;
} Lexer;

// Lexer functions
Lexer lexer_new(const char* source);
Token lexer_next(Lexer* lex);
bool lexer_is_end(Lexer* lex);
void lexer_skip_whitespace(Lexer* lex);
char lexer_advance(Lexer* lex);
char lexer_peek(Lexer* lex);
char lexer_peek_next(Lexer* lex);
bool lexer_match(Lexer* lex, char expected);
Token lexer_error(Lexer* lex, const char* msg);

#endif
