#include "lexer.h"
#include <string.h>
#include <ctype.h>

Lexer lexer_new(const char* source) {
    Lexer lex;
    lex.start = source;
    lex.current = source;
    lex.line = 1;
    lex.column = 0;
    lex.error = error_new(ERR_NONE, 0, 0, NULL);
    return lex;
}

bool lexer_is_end(Lexer* lex) {
    return *lex->current == '\0';
}

char lexer_advance(Lexer* lex) {
    lex->current++;
    lex->column++;
    return lex->current[-1];
}

char lexer_peek(Lexer* lex) {
    return *lex->current;
}

char lexer_peek_next(Lexer* lex) {
    if (lexer_is_end(lex)) return '\0';
    return lex->current[1];
}

bool lexer_match(Lexer* lex, char expected) {
    if (lexer_is_end(lex)) return false;
    if (*lex->current != expected) return false;
    lexer_advance(lex);
    return true;
}

void lexer_skip_whitespace(Lexer* lex) {
    for (;;) {
        char c = lexer_peek(lex);
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                lexer_advance(lex);
                break;
            case '\n':
                lex->line++;
                lex->column = 0;
                lexer_advance(lex);
                break;
            case '/':
                if (lexer_peek_next(lex) == '/') {
                    while (lexer_peek(lex) != '\n' && !lexer_is_end(lex)) {
                        lexer_advance(lex);
                    }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

static TokenType lexer_check_keyword(Lexer* lex, int start, int len, const char* rest, TokenType type) {
    if (lex->current - lex->start == start + len &&
        strncmp(lex->start + start, rest, len) == 0) {
        return type;
    }
    return TOKEN_IDENT;
}

static TokenType lexer_identifier_type(Lexer* lex) {
    int len = (int)(lex->current - lex->start);
    
    switch (len) {
        case 2: return lexer_check_keyword(lex, 0, 2, "in", TOKEN_IN);
        case 3:
            if (strncmp(lex->start, "let", 3) == 0) return TOKEN_LET;
            if (strncmp(lex->start, "mut", 3) == 0) return TOKEN_MUT;
            if (strncmp(lex->start, "fun", 3) == 0) return TOKEN_FUN;
            if (strncmp(lex->start, "for", 3) == 0) return TOKEN_FOR;
            if (strncmp(lex->start, "nil", 3) == 0) return TOKEN_NIL;
            return TOKEN_IDENT;
        case 4:
            if (strncmp(lex->start, "enum", 4) == 0) return TOKEN_ENUM;
            if (strncmp(lex->start, "type", 4) == 0) return TOKEN_TYPE;
            if (strncmp(lex->start, "else", 4) == 0) return TOKEN_ELSE;
            if (strncmp(lex->start, "when", 4) == 0) return TOKEN_WHEN;
            if (strncmp(lex->start, "is", 2) == 0 && lexer_peek(lex) == ' ') return TOKEN_IS;
            if (strncmp(lex->start, "not", 3) == 0) return TOKEN_NOT;
            if (strncmp(lex->start, "do", 2) == 0) return TOKEN_DO;
            if (strncmp(lex->start, "pub", 3) == 0) return TOKEN_PUB;
            if (strncmp(lex->start, "i8", 2) == 0) return TOKEN_I8;
            if (strncmp(lex->start, "f8", 2) == 0) return TOKEN_F8;
            return TOKEN_IDENT;
        case 5:
            if (strncmp(lex->start, "while", 5) == 0) return TOKEN_WHILE;
            if (strncmp(lex->start, "break", 5) == 0) return TOKEN_BREAK;
            if (strncmp(lex->start, "async", 5) == 0) return TOKEN_ASYNC;
            if (strncmp(lex->start, "await", 5) == 0) return TOKEN_AWAIT;
            if (strncmp(lex->start, "i16", 3) == 0) return TOKEN_I16;
            if (strncmp(lex->start, "f16", 3) == 0) return TOKEN_F16;
            return TOKEN_IDENT;
        case 6:
            if (strncmp(lex->start, "return", 6) == 0) return TOKEN_RETURN;
            if (strncmp(lex->start, "struct", 6) == 0) return TOKEN_STRUCT;
            if (strncmp(lex->start, "elif", 4) == 0) return TOKEN_ELIF;
            if (strncmp(lex->start, "i32", 3) == 0) return TOKEN_I32;
            if (strncmp(lex->start, "i64", 3) == 0) return TOKEN_I64;
            if (strncmp(lex->start, "f32", 3) == 0) return TOKEN_F32;
            if (strncmp(lex->start, "f64", 3) == 0) return TOKEN_F64;
            return TOKEN_IDENT;
        case 7:
            if (strncmp(lex->start, "default", 7) == 0) return TOKEN_DEFAULT;
            if (strncmp(lex->start, "list", 4) == 0) return TOKEN_LIST;
            if (strncmp(lex->start, "bool", 4) == 0) return TOKEN_BOOL;
            return TOKEN_IDENT;
        case 8:
            if (strncmp(lex->start, "continue", 8) == 0) return TOKEN_CONTINUE;
            if (strncmp(lex->start, "dyn", 3) == 0) return TOKEN_DYN;
            if (strncmp(lex->start, "num", 3) == 0) return TOKEN_NUM;
            if (strncmp(lex->start, "str", 3) == 0) return TOKEN_STR;
            if (strncmp(lex->start, "map", 3) == 0) return TOKEN_MAP;
            if (strncmp(lex->start, "set", 3) == 0) return TOKEN_SET;
            if (strncmp(lex->start, "void", 4) == 0) return TOKEN_VOID;
            return TOKEN_IDENT;
        case 9:
            if (strncmp(lex->start, "use", 3) == 0) return TOKEN_USE;
            if (strncmp(lex->start, "as", 2) == 0) return TOKEN_AS;
            return TOKEN_IDENT;
    }
    return TOKEN_IDENT;
}

static Token lexer_make_token(Lexer* lex, TokenType type) {
    return token_new(type, lex->start, 
                     (size_t)(lex->current - lex->start),
                     lex->line, lex->column);
}

static Token lexer_string(Lexer* lex) {
    while (lexer_peek(lex) != '"' && !lexer_is_end(lex)) {
        if (lexer_peek(lex) == '\n') lex->line++;
        lexer_advance(lex);
    }
    
    if (lexer_is_end(lex)) {
        return lexer_error(lex, "Unterminated string");
    }
    
    lexer_advance(lex); // closing "
    return lexer_make_token(lex, TOKEN_STRING);
}

static Token lexer_number(Lexer* lex) {
    while (isdigit(lexer_peek(lex))) {
        lexer_advance(lex);
    }
    
    if (lexer_peek(lex) == '.' && isdigit(lexer_peek_next(lex))) {
        lexer_advance(lex); // consume .
        while (isdigit(lexer_peek(lex))) {
            lexer_advance(lex);
        }
    }
    
    return lexer_make_token(lex, TOKEN_NUMBER);
}

static Token lexer_identifier(Lexer* lex) {
    while (isalnum(lexer_peek(lex)) || lexer_peek(lex) == '_') {
        lexer_advance(lex);
    }
    
    return lexer_make_token(lex, lexer_identifier_type(lex));
}

Token lexer_next(Lexer* lex) {
    lexer_skip_whitespace(lex);
    lex->start = lex->current;
    
    if (lexer_is_end(lex)) {
        return lexer_make_token(lex, TOKEN_EOF);
    }
    
    char c = lexer_advance(lex);
    
    if (isalpha(c) || c == '_') {
        return lexer_identifier(lex);
    }
    
    if (isdigit(c)) {
        return lexer_number(lex);
    }
    
    switch (c) {
        case '"': return lexer_string(lex);
        
        case '(': return lexer_make_token(lex, TOKEN_LPAREN);
        case ')': return lexer_make_token(lex, TOKEN_RPAREN);
        case '{': return lexer_make_token(lex, TOKEN_LBRACE);
        case '}': return lexer_make_token(lex, TOKEN_RBRACE);
        case '[': return lexer_make_token(lex, TOKEN_LBRACKET);
        case ']': return lexer_make_token(lex, TOKEN_RBRACKET);
        case ',': return lexer_make_token(lex, TOKEN_COMMA);
        case '.': return lexer_make_token(lex, TOKEN_DOT);
        case ';': return lexer_make_token(lex, TOKEN_SEMICOLON);
        case ':': return lexer_make_token(lex, TOKEN_COLON);
        
        case '+': return lexer_make_token(lex, TOKEN_PLUS);
        case '-': return lexer_make_token(lex, TOKEN_MINUS);
        case '*': return lexer_make_token(lex, TOKEN_STAR);
        case '/': return lexer_make_token(lex, TOKEN_SLASH);
        
        case '=':
            if (lexer_match(lex, '=')) return lexer_make_token(lex, TOKEN_EQ);
            if (lexer_match(lex, '>')) return lexer_make_token(lex, TOKEN_ARROW);
            return lexer_make_token(lex, TOKEN_ASSIGN);
        
        case '!':
            if (lexer_match(lex, '=')) return lexer_make_token(lex, TOKEN_NEQ);
            return lexer_error(lex, "Expected '=' after '!'");
        
        case '<':
            if (lexer_match(lex, '=')) return lexer_make_token(lex, TOKEN_LTE);
            return lexer_make_token(lex, TOKEN_LT);
        
        case '>':
            if (lexer_match(lex, '=')) return lexer_make_token(lex, TOKEN_GTE);
            return lexer_make_token(lex, TOKEN_GT);
    }
    
    return lexer_error(lex, "Unexpected character");
}

Token lexer_error(Lexer* lex, const char* msg) {
    lex->error = error_fatal(ERR_LEXER, lex->line, lex->column, msg);
    return token_new(TOKEN_ERROR, lex->start, 0, lex->line, lex->column);
}
