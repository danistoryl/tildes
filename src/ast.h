#ifndef INTERPRETER_AST_H
#define INTERPRETER_AST_H

#include "token.h"
#include "value.h"
#include <stddef.h>

// Forward declarations
typedef struct ASTNode ASTNode;
typedef struct Expr Expr;
typedef struct Stmt Stmt;

// --- Expression Types ---
typedef enum {
    EXPR_LITERAL,
    EXPR_VARIABLE,
    EXPR_BINARY,
    EXPR_UNARY,
    EXPR_CALL,
    EXPR_GET,
    EXPR_SET,
    EXPR_LIST,
    EXPR_MAP,
    EXPR_RECORD,
    EXPR_CAST,
    EXPR_AWAIT
} ExprType;

// --- Statement Types ---
typedef enum {
    STMT_BLOCK,
    STMT_VAR_DECL,
    STMT_FUN_DECL,
    STMT_RETURN,
    STMT_IF,
    STMT_WHEN,
    STMT_FOR,
    STMT_WHILE,
    STMT_DO_WHILE,
    STMT_BREAK,
    STMT_CONTINUE,
    STMT_USE,
    STMT_ENUM,
    STMT_STRUCT,
    STMT_TYPE_ALIAS,
    STMT_ASYNC
} StmtType;

// --- Expression Struct ---
struct Expr {
    ExprType type;
    Token token;
    union {
        Value literal;
        struct {
            char* name;
        } variable;
        struct {
            Expr* left;
            Expr* right;
            TokenType op;
        } binary;
        struct {
            Expr* operand;
            TokenType op;
        } unary;
        struct {
            Expr* callee;
            Expr** args;
            size_t arg_count;
        } call;
        struct {
            Expr* object;
            char* name;
        } get;
        struct {
            Expr* object;
            char* name;
            Expr* value;
        } set;
        struct {
            Expr** items;
            size_t count;
        } list;
        struct {
            Expr** keys;
            Expr** values;
            size_t count;
        } map;
        struct {
            Expr** fields;
            size_t count;
        } record;
        struct {
            Expr* expr;
            TypeKind target_type;
        } cast;
        struct {
            Expr* task;
        } await;
    };
};

// --- Statement Struct ---
struct Stmt {
    StmtType type;
    Token token;
    union {
        struct {
            Stmt** statements;
            size_t count;
        } block;
        struct {
            char* name;
            TypeKind var_type;
            bool is_mutable;
            bool is_pub;
            Expr* initializer;
        } var_decl;
        struct {
            char* name;
            bool is_async;
            bool is_pub;
            char** params;
            TypeKind* param_types;
            size_t param_count;
            TypeKind return_type;
            Stmt* body;
        } fun_decl;
        struct {
            Expr* value;
        } return_stmt;
        struct {
            Expr* condition;
            Stmt* then_branch;
            Stmt* else_branch;
        } if_stmt;
        struct {
            Expr* subject;
            Expr** cases;
            Stmt** branches;
            size_t case_count;
        } when_stmt;
        struct {
            char* var_name;
            Expr* iterable;
            Stmt* body;
        } for_stmt;
        struct {
            Expr* condition;
            Stmt* body;
        } while_stmt;
        struct {
            Expr* condition;
            Stmt* body;
        } do_while_stmt;
        struct {
            char* label;
        } break_stmt;
        struct {
            char* label;
        } continue_stmt;
        struct {
            char* module;
        } use_stmt;
        struct {
            char* name;
            char** variants;
            size_t variant_count;
        } enum_decl;
        struct {
            char* name;
            char** field_names;
            TypeKind* field_types;
            size_t field_count;
        } struct_decl;
        struct {
            char* name;
            TypeKind type;
        } type_alias;
        struct {
            Stmt* async_body;
        } async_stmt;
    };
};

// AST Node wrapper
struct ASTNode {
    bool is_expr;
    union {
        Expr* expr;
        Stmt* stmt;
    };
};

// Parser functions
ASTNode* parse(const char* source, Error* error);
Expr* parse_expression(const char* source, Error* error);
Stmt* parse_statement(const char* source, Error* error);

#endif
