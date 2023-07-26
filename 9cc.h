#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// tokenizer.c

typedef enum {
    TK_PUNCT,
    TK_NUM,
    TK_EOF,
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

typedef enum {
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_NEG, // unary -
    ND_EQ,  // ==
    ND_NE,  // !=
    ND_LT,  // <
    ND_LE,  // <=
    ND_NUM, // Integer
} NodeKind;

typedef struct Node Node;
struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
};

Node *parse(Token *tok);

// codegen.c

void codegen(Node *node);
