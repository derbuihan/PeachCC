#include "peachcc.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        error("%s: 引数の個数が正しくありません。", argv[0]);
    }

    // Tokenize and parse.
    Token *tok = tokenize_file(argv[1]);
    Obj *prog = parse(tok);

    // Traverse the AST to emit assembly.
    codegen(prog);

    return 0;
}
