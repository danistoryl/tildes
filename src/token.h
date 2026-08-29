#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include "types.h"
#include <stddef.h>

// --- Token Types ---
typedef enum {
    // Literals
    TOKEN_IDENT,
    TOKEN_NUMBER,
    TOKEN_STRING,
    
    // Keywords
    TOKEN_LET, TOKEN_MUT, TOKEN_PUB,
    TOKEN_BOOL, TOKEN_NUM, TOKEN_STR,
    TOKEN_I8, TOKEN_I16, TOKEN_I32, TOKEN_I64,
    TOKEN_F8, TOKEN_F16, TOKEN_F32, TOKEN_F64,
    TOKEN_DYN, TOKEN_NIL, TOKEN_VOID,
    TOKEN_LIST, TOKEN_MAP, TOKEN_SET,
    TOKEN_ENUM, TOKEN_TYPE, TOKEN_STRUCT,
    TOKEN_FUN, TOKEN_RETURN,
    TOKEN_IF, TOKEN_ELIF, TOKEN_ELSE,
    TOKEN_WHEN, TOKEN_IS, TOKEN_NOT,
    TOKEN_FOR, TOKEN_IN, TOKEN_WHILE, TOKEN_DO,
    TOKEN_BREAK, TOKEN_CONTINUE, TOKEN_DEFAULT,
    TOKEN_USE, TOKEN_AS,
    TOKEN_ASYNC, TOKEN_AWAIT,
    
    // Operators
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH,
    TOKEN_EQ, TOKEN_NEQ, TOKEN_LT, TOKEN_GT, TOKEN_LTE, TOKEN_GTE,
    TOKEN_ASSIGN, TOKEN_ARROW, TOKEN_DOT, TOKEN_COLON,
    TOKEN_RANGE, TOKEN_COMMA, TOKEN_SEMICOLON,
    
    // Delimiters
    TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_LBRACKET, TOKEN_RBRACKET,
    
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

// --- Token Struct ---
typedef struct {
    TokenType type;
    const char* start;
    size_t length;
    int line;
    int column;
} Token;

// Token functions
Token token_new(TokenType type, const char* start, size_t len, int line, int col);
const char* token_type_str(TokenType type);
bool token_is_keyword(const char* str, size_t len);

#endif
