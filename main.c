#include "peachcc.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        error("%s: 引数の個数が正しくありません。", argv[0]);
    }

    Token *tok = tokenize(argv[1]);
    Function *prog = parse(tok);
    codegen(prog);

    return 0;
}