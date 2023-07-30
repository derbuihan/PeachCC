#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// tokenizer.c

typedef enum {
    TK_IDENT, // Identifiers
    TK_PUNCT, // Punctuators
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

Token *tokenize(char *p);

// parser.c

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
    ND_EXPR_STMT, // Expression statement
    ND_VAR,       // Variable
    ND_NUM,       // Integer
} NodeKind;

typedef struct Node Node;
struct Node {
    NodeKind kind; // Node kind
    Node *next;    // Next node
    Node *lhs;     // Lift-hand side
    Node *rhs;     // Right-hand side
    char name;     // Used if kind == ND_VAR
    int val;       // Used if kind == ND_NUM
};

Node *parse(Token *tok);

// codegen.c

void codegen(Node *node);
