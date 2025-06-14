#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "tlist.h"

// ===================== TOKEN STRINGS =====================

// Keywords
static const char* const keywords[] = {"void", "char",  "short", "int",      "long",  "return", "if",
                                       "else", "while", "for",   "continue", "break", "signed", "unsigned"};

// Assignment operators
static const char* const assignment_operators[] = {"=", "+=", "-=", "*=", "/=", "%=", "|=", "&=", "^=", "<<=", ">>="};

// Relational operators
static const char* const relational_operators[] = {"==", "!=", "<", "<=", ">", ">="};

// Logic operators
static const char* const logic_operators[] = {"!", "||", "&&"};

// Bitwise operators
static const char* const bitwise_operators[] = {"~", "|", "&", "^", "<<", ">>"};

// Arithmetic operators
static const char* const arithmetic_operators[] = {"+", "-", "*", "/", "%"};

// Separators
static const char* const separators[] = {"'", "\"", "?", "(", ")", "[", "]", "{", "}", ",", ":", ";"};

// ===================== MATCHING ========================

typedef enum match { MATCH_ERR = -1, MATCH_NONE = 0, MATCH_PARTIAL = 1, MATCH_FULL = 2, MATCH_FULL_DIFF = 3 } match_t;
const char* match_to_string(match_t m);

// ===================== MATCH_NODATA =====================

match_t match_keyword(char** strp, token_t* tp);
match_t match_assignment_operator(char** strp, token_t* tp);
match_t match_relational_operator(char** strp, token_t* tp);
match_t match_logic_operator(char** strp, token_t* tp);
match_t match_bitwise_operator(char** strp, token_t* tp);
match_t match_arithmetic_operator(char** strp, token_t* tp);
match_t match_separator(char** strp, token_t* tp);

// =================== MATCH DATA ======================
match_t match_char_literal(char** strp, token_t* tp);
match_t match_integer_literal(char** strp, token_t* tp);
match_t match_string_literal(char** strp, token_t* tp);
match_t match_identifier(char** strp, token_t* tp);

// ==================== TOKENIZER ========================

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