#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "tlist.h"

typedef struct tokenizer _tokenizer, *tokenizer_t;

// Creates a new tokenizer
tokenizer_t tokenizer_new();

// Tokenizes the file (if found) with the tokenizer t
int tokenize(tokenizer_t t, const char* filename);

// Gets a list of tokens from a tokenizer,
// which will be left with no tokens.
tlist_t get_tokens(tokenizer_t t);

void tokenizer_free(tokenizer_t* tp);

#endif