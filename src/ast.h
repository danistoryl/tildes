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

// --- Expression Data Structs (Small, Focused) ---

typedef struct ExprLiteral {
    Value value;
} ExprLiteral;

typedef struct ExprVariable {
    char* name;
} ExprVariable;

typedef struct ExprBinary {
    Expr* left;
    Expr* right;
    TokenType op;
} ExprBinary;

typedef struct ExprUnary {
    Expr* operand;
    TokenType op;
} ExprUnary;

typedef struct ExprCall {
    Expr* callee;
    Expr** args;
    size_t arg_count;
} ExprCall;

typedef struct ExprGet {
    Expr* object;
    char* name;
} ExprGet;

typedef struct ExprSet {
    Expr* object;
    char* name;
    Expr* value;
} ExprSet;

typedef struct ExprList {
    Expr** items;
    size_t count;
} ExprList;

typedef struct ExprMap {
    Expr** keys;
    Expr** values;
    size_t count;
} ExprMap;

typedef struct ExprRecord {
    Expr** fields;
    size_t count;
} ExprRecord;

typedef struct ExprCast {
    Expr* expr;
    TypeKind target_type;
} ExprCast;

typedef struct ExprAwait {
    Expr* task;
} ExprAwait;

// --- Expression Struct ---
struct Expr {
    ExprType type;
    Token token;
    union {
        ExprLiteral literal;
        ExprVariable variable;
        ExprBinary binary;
        ExprUnary unary;
        ExprCall call;
        ExprGet get;
        ExprSet set;
        ExprList list;
        ExprMap map;
        ExprRecord record;
        ExprCast cast;
        ExprAwait await;
    };
};

// --- Statement Data Structs (Small, Focused) ---

typedef struct StmtBlock {
    Stmt** statements;
    size_t count;
} StmtBlock;

typedef struct StmtVarDecl {
    char* name;
    TypeKind var_type;
    bool is_mutable;
    bool is_pub;
    Expr* initializer;
} StmtVarDecl;

typedef struct StmtFunDecl {
    char* name;
    bool is_async;
    bool is_pub;
    char** params;
    TypeKind* param_types;
    size_t param_count;
    TypeKind return_type;
    Stmt* body;
} StmtFunDecl;

typedef struct StmtReturn {
    Expr* value;
} StmtReturn;

typedef struct StmtIf {
    Expr* condition;
    Stmt* then_branch;
    Stmt* else_branch;
} StmtIf;

typedef struct StmtWhen {
    Expr* subject;
    Expr** cases;
    Stmt** branches;
    size_t case_count;
} StmtWhen;

typedef struct StmtFor {
    char* var_name;
    Expr* iterable;
    Stmt* body;
} StmtFor;

typedef struct StmtWhile {
    Expr* condition;
    Stmt* body;
} StmtWhile;

typedef struct StmtDoWhile {
    Expr* condition;
    Stmt* body;
} StmtDoWhile;

typedef struct StmtBreak {
    char* label;
} StmtBreak;

typedef struct StmtContinue {
    char* label;
} StmtContinue;

typedef struct StmtUse {
    char* module;
} StmtUse;

typedef struct StmtEnum {
    char* name;
    char** variants;
    size_t variant_count;
} StmtEnum;

typedef struct StmtStruct {
    char* name;
    char** field_names;
    TypeKind* field_types;
    size_t field_count;
} StmtStruct;

typedef struct StmtTypeAlias {
    char* name;
    TypeKind type;
} StmtTypeAlias;

typedef struct StmtAsync {
    Stmt* async_body;
} StmtAsync;

// --- Statement Struct ---
struct Stmt {
    StmtType type;
    Token token;
    union {
        StmtBlock block;
        StmtVarDecl var_decl;
        StmtFunDecl fun_decl;
        StmtReturn return_stmt;
        StmtIf if_stmt;
        StmtWhen when_stmt;
        StmtFor for_stmt;
        StmtWhile while_stmt;
        StmtDoWhile do_while_stmt;
        StmtBreak break_stmt;
        StmtContinue continue_stmt;
        StmtUse use_stmt;
        StmtEnum enum_decl;
        StmtStruct struct_decl;
        StmtTypeAlias type_alias;
        StmtAsync async_stmt;
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
