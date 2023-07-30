#include "peachcc.h"

static Node *expr(Token **rest, Token *tok);

static Node *expr_stmt(Token **rest, Token *tok);

static Node *equality(Token **rest, Token *tok);

static Node *relational(Token **rest, Token *tok);

static Node *add(Token **rest, Token *tok);

static Node *mul(Token **rest, Token *tok);

static Node *unary(Token **rest, Token *tok);

static Node *primary(Token **rest, Token *tok);

bool equal(Token *tok, char *op) {
    return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

Token *skip(Token *tok, char *s) {
    if (!equal(tok, s)) {
        error_tok(tok, "expect '%s'", s);
    }
    return tok->next;
}

static Node *new_node(NodeKind kind) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    return node;
}

static Node *new_binary(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = new_node(kind);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

static Node *new_unary(NodeKind kind, Node *expr) {
    Node *node = new_node(kind);
    node->lhs = expr;
    return node;
}

static Node *new_num(int val) {
    Node *node = new_node(ND_NUM);
    node->val = val;
    return node;
}

// stmt = expr-stmt
static Node *stmt(Token **rest, Token *tok) {
    return expr_stmt(rest, tok);
}

static Node *expr_stmt(Token **rest, Token *tok) {
    Node *node = new_unary(ND_EXPR_STMT, expr(&tok, tok));
    *rest = skip(tok, ";");
    return node;
}

// expr = equality
static Node *expr(Token **rest, Token *tok) {
    return equality(rest, tok);
}

// equality = relational ("==" relational | "!=" relational)*
static Node *equality(Token **rest, Token *tok) {
    Node *node = relational(&tok, tok);

    for (;;) {
        if (equal(tok, "==")) {
            node = new_binary(
                    ND_EQ,
                    node,
                    relational(&tok, tok->next));
            continue;
        }
        if (equal(tok, "!=")) {
            node = new_binary(
                    ND_NE,
                    node,
                    relational(&tok, tok->next));
            continue;
        }

        *rest = tok;
        return node;
    }
}

// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
static Node *relational(Token **rest, Token *tok) {
    Node *node = add(&tok, tok);

    for (;;) {
        if (equal(tok, "<")) {
            node = new_binary(
                    ND_LT,
                    node,
                    add(&tok, tok->next));
            continue;
        }
        if (equal(tok, "<=")) {
            node = new_binary(
                    ND_LE,
                    node,
                    add(&tok, tok->next));
            continue;
        }
        if (equal(tok, ">")) {
            node = new_binary(
                    ND_LT,
                    add(&tok, tok->next),
                    node);
            continue;
        }
        if (equal(tok, ">=")) {
            node = new_binary(
                    ND_LE,
                    add(&tok, tok->next),
                    node);
            continue;
        }

        *rest = tok;
        return node;
    }
}


// add = mul ("+" mul | "-" mul)*
static Node *add(Token **rest, Token *tok) {
    Node *node = mul(&tok, tok);

    for (;;) {
        if (equal(tok, "+")) {
            node = new_binary(
                    ND_ADD,
                    node,
                    mul(&tok, tok->next));
            continue;
        }
        if (equal(tok, "-")) {
            node = new_binary(
                    ND_SUB,
                    node,
                    mul(&tok, tok->next));
            continue;
        }

        *rest = tok;
        return node;
    }
}

// mul = unary ("*" unary | "/" unary)*
static Node *mul(Token **rest, Token *tok) {
    Node *node = unary(&tok, tok);

    for (;;) {
        if (equal(tok, "*")) {
            node = new_binary(
                    ND_MUL,
                    node,
                    unary(&tok, tok->next));
            continue;
        }

        if (equal(tok, "/")) {
            node = new_binary(
                    ND_DIV,
                    node,
                    unary(&tok, tok->next));
            continue;
        }

        *rest = tok;
        return node;
    }
}

// unary = ("+" | "-")? primary
static Node *unary(Token **rest, Token *tok) {
    if (equal(tok, "+")) {
        return unary(rest, tok->next);
    }

    if (equal(tok, "-")) {
        return new_unary(ND_NEG, unary(rest, tok->next));
    }

    return primary(rest, tok);
}

// primary = num | "(" expr ")"
static Node *primary(Token **rest, Token *tok) {
    if (equal(tok, "(")) {
        Node *node = expr(&tok, tok->next);
        *rest = skip(tok, ")");
        return node;
    }

    if (tok->kind == TK_NUM) {
        Node *node = new_num(tok->val);
        *rest = tok->next;
        return node;
    }

    error_at(tok->loc, "数値でも開きカッコでもないトークンです");
}

// program = stmt*
Node *parse(Token *tok) {
    Node head = {};
    Node *cur = &head;
    while (tok->kind != TK_EOF) {
        cur = cur->next = stmt(&tok, tok);
    }
    return head.next;
}