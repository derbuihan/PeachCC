#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

// tokenizer.c

typedef enum {
    TK_IDENT, // Identifiers
    TK_PUNCT, // Punctuators
    TK_KEYWORD, // Keywords
    TK_NUM,   // Numeric literals
    TK_EOF,   // End-of-file markers
} TokenKind;

typedef struct Token Token;
struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *loc;
    int len;
};

void error(char *fmt, ...);

void error_at(char *loc, char *fmt, ...);

void error_tok(Token *tok, char *fmt, ...);

bool equal(Token *tok, char *op);

Token *skip(Token *tok, char *s);

Token *tokenize(char *p);

// parser.c

typedef struct Obj Obj;
struct Obj {
    Obj *next;
    char *name; // Variable name
    int offset; // Offset from RBP
};

typedef struct Function Function;
struct Function {
    Node *body;
    Obj *locals;
    int stack_size;
};

typedef enum {
    ND_ADD,       // +
    ND_SUB,       // -
    ND_MUL,       // *
    ND_DIV,       // /
    ND_NEG,       // unary -
    ND_EQ,        // ==
    ND_NE,        // !=
    ND_LT,        // <
    ND_LE,        // <=
    ND_ASSIGN,    // =
    ND_RETURN,    // "return"
    ND_IF,        // "if"
    ND_BLOCK,     // { ... }
    ND_EXPR_STMT, // Expression statement
    ND_VAR,       // Variable
    ND_NUM,       // Integer
} NodeKind;

struct Node {
    NodeKind kind; // Node kind
    Node *next;    // Next node

    Node *lhs;     // Lift-hand side
    Node *rhs;     // Right-hand side

    // "if" statement
    Node *cond;
    Node *then;
    Node *els;

    // Block
    Node *body;

    Obj *var;      // Used if kind == ND_VAR
    int val;       // Used if kind == ND_NUM
};

Function *parse(Token *tok);

// codegen.c

void codegen(Function *prog);
