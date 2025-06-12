#include "tokenization/tokenizer.h"
#include <stdio.h>
#include "tests.h"

int main(int argc, char** args) {
    // if (argc != 2) {
    //     fprintf(stderr, "Wrong number of arguments! Usage: disa <file>, where <file> is a C file to compile\n");
    //     return 1;
    // }

    // tokenizer_t tokenizer = tokenizer_new();
    // tokenize(tokenizer, args[1]);

    // tlist_t tokens = get_tokens(tokenizer);
    // tlist_print(tokens);
    // tlist_free(&tokens);

    // tokenizer_free(&tokenizer);

    run_tests();

    return 0;
}