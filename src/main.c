#include "tokenization/tokenizer.h"
#include <stdio.h>
#include "utils/str.h"

int main(int argc, char** args) {
    if (argc != 2) {
        return 1;
    }

    tokenizer_t tokenizer = tokenizer_new();
    if (!tokenize(tokenizer, args[1])) {
        printf("Error while tokenizing\n");
    }

    tlist_t tokens = get_tokens(tokenizer);
    tlist_print(tokens);
    tlist_free(&tokens);

    tokenizer_free(&tokenizer);

    return 0;
}