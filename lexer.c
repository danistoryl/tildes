/*
 * lexer.c - Lexical analyzer
 * Converts source code to tokens
 * Short, focused functions
 */

#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

static char current_char(const Lexer *lexer) {
    if (lexer->current >= lexer->length) {
        return '\0';
    }
    return lexer->source[lexer->current];
}

static void advance(Lexer *lexer) {
    if (current_char(lexer) == '\n') {
        lexer->line++;
        lexer->column = 0;
    } else {
        lexer->column++;
    }
    lexer->current++;
}

static void skip_whitespace(Lexer *lexer) {
    while (isspace(current_char(lexer))) {
        advance(lexer);
    }
}

static void skip_comment(Lexer *lexer) {
    /* Skip single-line comments starting with // */
    if (current_char(lexer) == '/' && 
        lexer->current + 1 < lexer->length &&
        lexer->source[lexer->current + 1] == '/') {
        while (current_char(lexer) != '\n' && 
               current_char(lexer) != '\0') {
            advance(lexer);
        }
    }
}

static Token make_token(Lexer *lexer, TokenType type, const char *value) {
    return token_create(type, value, lexer->line, lexer->column);
}

static Token number_literal(Lexer *lexer) {
    size_t start = lexer->current;
    
    while (isdigit(current_char(lexer))) {
        advance(lexer);
    }
    
    if (current_char(lexer) == '.') {
        advance(lexer);
        while (isdigit(current_char(lexer))) {
            advance(lexer);
        }
    }
    
    size_t length = lexer->current - start;
    char *num_str = malloc(length + 1);
    if (num_str != NULL) {
        strncpy(num_str, lexer->source + start, length);
        num_str[length] = '\0';
    }
    
    return make_token(lexer, TOKEN_NUMBER, num_str);
}

static Token string_literal(Lexer *lexer) {
    advance(lexer); /* Skip opening quote */
    size_t start = lexer->current;
    
    while (current_char(lexer) != '"' && 
           current_char(lexer) != '\0') {
        advance(lexer);
    }
    
    size_t length = lexer->current - start;
    char *str_val = malloc(length + 1);
    if (str_val != NULL) {
        strncpy(str_val, lexer->source + start, length);
        str_val[length] = '\0';
    }
    
    if (current_char(lexer) == '"') {
        advance(lexer);
    }
    
    return make_token(lexer, TOKEN_STRING, str_val);
}

static Token identifier_or_keyword(Lexer *lexer) {
    size_t start = lexer->current;
    
    while (isalnum(current_char(lexer)) || 
           current_char(lexer) == '_') {
        advance(lexer);
    }
    
    size_t length = lexer->current - start;
    char *ident = malloc(length + 1);
    if (ident != NULL) {
        strncpy(ident, lexer->source + start, length);
        ident[length] = '\0';
    }
    
    /* Check for keywords */
    if (strcmp(ident, "print") == 0) {
        return make_token(lexer, TOKEN_PRINT, ident);
    } else if (strcmp(ident, "let") == 0) {
        return make_token(lexer, TOKEN_LET, ident);
    } else if (strcmp(ident, "if") == 0) {
        return make_token(lexer, TOKEN_IF, ident);
    } else if (strcmp(ident, "else") == 0) {
        return make_token(lexer, TOKEN_ELSE, ident);
    } else if (strcmp(ident, "while") == 0) {
        return make_token(lexer, TOKEN_WHILE, ident);
    }
    
    return make_token(lexer, TOKEN_IDENTIFIER, ident);
}

Lexer lexer_init(const char *source) {
    Lexer lexer;
    lexer.source = source;
    lexer.length = strlen(source);
    lexer.start = 0;
    lexer.current = 0;
    lexer.line = 1;
    lexer.column = 0;
    return lexer;
}

Token lexer_next_token(Lexer *lexer) {
    skip_whitespace(lexer);
    skip_comment(lexer);
    skip_whitespace(lexer); /* Skip whitespace after comment */
    
    if (lexer->current >= lexer->length) {
        return make_token(lexer, TOKEN_EOF, NULL);
    }
    
    char c = current_char(lexer);
    
    if (isdigit(c)) {
        return number_literal(lexer);
    }
    
    if (c == '"') {
        return string_literal(lexer);
    }
    
    if (isalpha(c) || c == '_') {
        return identifier_or_keyword(lexer);
    }
    
    switch (c) {
        case '+':
            advance(lexer);
            return make_token(lexer, TOKEN_PLUS, "+");
        case '-':
            advance(lexer);
            return make_token(lexer, TOKEN_MINUS, "-");
        case '*':
            advance(lexer);
            return make_token(lexer, TOKEN_STAR, "*");
        case '/':
            advance(lexer);
            return make_token(lexer, TOKEN_SLASH, "/");
        case '=':
            advance(lexer);
            return make_token(lexer, TOKEN_ASSIGN, "=");
        case '(':
            advance(lexer);
            return make_token(lexer, TOKEN_LPAREN, "(");
        case ')':
            advance(lexer);
            return make_token(lexer, TOKEN_RPAREN, ")");
        case '{':
            advance(lexer);
            return make_token(lexer, TOKEN_LBRACE, "{");
        case '}':
            advance(lexer);
            return make_token(lexer, TOKEN_RBRACE, "}");
        case ';':
            advance(lexer);
            return make_token(lexer, TOKEN_SEMICOLON, ";");
        default:
            advance(lexer);
            return make_token(lexer, TOKEN_UNKNOWN, "?");
    }
}

void lexer_free(Lexer *lexer) {
    (void)lexer; /* Nothing to free in lexer itself */
}
