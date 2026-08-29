#include "token.h"
#include <string.h>

static const char* token_names[] = {
    "IDENT", "NUMBER", "STRING",
    "LET", "MUT", "PUB",
    "BOOL", "NUM", "STR",
    "I8", "I16", "I32", "I64",
    "F8", "F16", "F32", "F64",
    "DYN", "NIL", "VOID",
    "LIST", "MAP", "SET",
    "ENUM", "TYPE", "STRUCT",
    "FUN", "RETURN",
    "IF", "ELIF", "ELSE",
    "WHEN", "IS", "NOT",
    "FOR", "IN", "WHILE", "DO",
    "BREAK", "CONTINUE", "DEFAULT",
    "USE", "AS",
    "ASYNC", "AWAIT",
    "+", "-", "*", "/",
    "==", "!=", "<", ">", "<=", ">=",
    "=", "=>", ".", ":",
    "..", ",", ";",
    "(", ")", "{", "}", "[", "]",
    "EOF", "ERROR"
};

Token token_new(TokenType type, const char* start, size_t len, int line, int col) {
    Token t;
    t.type = type;
    t.start = start;
    t.length = len;
    t.line = line;
    t.column = col;
    return t;
}

const char* token_type_str(TokenType type) {
    if (type < 0 || type > TOKEN_ERROR) {
        return "UNKNOWN";
    }
    return token_names[type];
}

bool token_is_keyword(const char* str, size_t len) {
    static const char* keywords[] = {
        "let", "mut", "pub", "bool", "num", "str",
        "i8", "i16", "i32", "i64", "f8", "f16", "f32", "f64",
        "dyn", "nil", "void", "list", "map", "set",
        "enum", "type", "struct", "fun", "return",
        "if", "elif", "else", "when", "is", "not",
        "for", "in", "while", "do", "break", "continue", "default",
        "use", "as", "async", "await"
    };
    
    for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (len == strlen(keywords[i]) && strncmp(str, keywords[i], len) == 0) {
            return true;
        }
    }
    return false;
}
