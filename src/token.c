/*
 * token.c - Token creation and management
 * Minimal macros, clean separation of concerns
 */

#define _POSIX_C_SOURCE 200809L

#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Token token_create(TokenType type, const char *value, size_t line, size_t column) {
    Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    
    if (value != NULL) {
        token.value = strdup(value);
    } else {
        token.value = NULL;
    }
    
    return token;
}

void token_free(Token *token) {
    if (token == NULL) {
        return;
    }
    
    free(token->value);
    token->value = NULL;
}

const char *token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_EOF:
            return "EOF";
        case TOKEN_NUMBER:
            return "NUMBER";
        case TOKEN_STRING:
            return "STRING";
        case TOKEN_IDENTIFIER:
            return "IDENTIFIER";
        case TOKEN_PLUS:
            return "PLUS";
        case TOKEN_MINUS:
            return "MINUS";
        case TOKEN_STAR:
            return "STAR";
        case TOKEN_SLASH:
            return "SLASH";
        case TOKEN_ASSIGN:
            return "ASSIGN";
        case TOKEN_LPAREN:
            return "LPAREN";
        case TOKEN_RPAREN:
            return "RPAREN";
        case TOKEN_LBRACE:
            return "LBRACE";
        case TOKEN_RBRACE:
            return "RBRACE";
        case TOKEN_SEMICOLON:
            return "SEMICOLON";
        case TOKEN_PRINT:
            return "PRINT";
        case TOKEN_LET:
            return "LET";
        case TOKEN_IF:
            return "IF";
        case TOKEN_ELSE:
            return "ELSE";
        case TOKEN_WHILE:
            return "WHILE";
        default:
            return "UNKNOWN";
    }
}
