/*
 * interpreter.h - Main header for the interpreter
 * Modern C interpreter with minimal macros
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stddef.h>
#include <stdbool.h>

/* Token types */
typedef enum {
    TOKEN_EOF,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_IDENTIFIER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_ASSIGN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMICOLON,
    TOKEN_PRINT,
    TOKEN_LET,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_UNKNOWN
} TokenType;

/* Token structure */
typedef struct {
    TokenType type;
    char *value;
    size_t line;
    size_t column;
} Token;

/* Value types for runtime */
typedef enum {
    VALUE_NUMBER,
    VALUE_STRING,
    VALUE_BOOL,
    VALUE_NULL
} ValueType;

/* Runtime value */
typedef struct {
    ValueType type;
    union {
        double number;
        char *string;
        bool boolean;
    } data;
} Value;

/* Function declarations */
Token token_create(TokenType type, const char *value, size_t line, size_t column);
void token_free(Token *token);
const char *token_type_name(TokenType type);

Value value_number(double num);
Value value_string(const char *str);
Value value_bool(bool b);
Value value_null(void);
void value_free(Value *val);

#endif /* INTERPRETER_H */
