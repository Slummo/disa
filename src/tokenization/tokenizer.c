#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

#define PARTIAL_SIZE 128
#define CHUNK_SIZE 16

struct tokenizer {
    // The list of tokens
    tlist_t tokens;

    //  A character buffer to accumulate
    // partial tokens across read operations
    char partial[PARTIAL_SIZE];
};

// Creates a new tokenizer
tokenizer_t tokenizer_new() {
    tokenizer_t t = (tokenizer_t)malloc(sizeof(_tokenizer));
    if (!t) {
        perror("Error with malloc");
        return NULL;
    }

    t->tokens = NULL;
    t->partial[0] = '\0';

    return t;
}

// Tokenizes the file
int tokenize(tokenizer_t t, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Error opening file");
        return 0;
    }

    char chunk[CHUNK_SIZE];
    while (!feof(f)) {
        size_t read = fread(chunk, sizeof(char), sizeof(char) * CHUNK_SIZE, f);
    }

    t->partial[0] = '\0';
    return 1;
}

// Gets a list of tokens from a tokenizer,
// which will be left with no tokens.
tlist_t get_tokens(tokenizer_t t) {
    tlist_t tokens = t->tokens;
    t->tokens = NULL;
    return tokens;
}

void tokenizer_free(tokenizer_t* tp) {
    if (!tp || !*tp) {
        return;
    }

    tlist_free(&(*tp)->tokens);
    free(*tp);

    *tp = NULL;
}