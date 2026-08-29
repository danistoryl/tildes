#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H

#include "ast.h"
#include "lexer.h"

// --- Parser State ---
typedef struct {
    Lexer lexer;
    Token current;
    Token previous;
    Error error;
    bool had_error;
    bool panic_mode;
} Parser;

// Parser functions
Parser parser_new(const char* source);
void parser_advance(Parser* p);
bool parser_check(Parser* p, TokenType type);
bool parser_match(Parser* p, TokenType type);
Token parser_consume(Parser* p, TokenType type, const char* msg);
Expr* parser_parse_precedence(Parser* p, int precedence);
Stmt* parser_parse_declaration(Parser* p);
Stmt* parser_parse_statement(Parser* p);
Expr* parser_parse_expression(Parser* p);
ASTNode* parser_parse(Parser* p);

#endif
