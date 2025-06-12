#ifndef STR_H
#define STR_H

#include "tokenization/token.h"

void skip_spaces(char** strp);

// Advances a char* pointer without making checks
void str_advance(char** strp, int amount);

/// @brief Helper function to match a token with no additional data
/// @param strp Pointer to the string containing the possible token
/// @param tp Pointer to the token_t to store the result
/// @param types Array of strings for the token types strings
/// @param types_len Length of types array
/// @param enum_offset Offset of the token_type_t enum to correctly
/// parse an index while looping through the types strings.
/// e.g. types = {*keywords*} => enum_offset = K_VOID
/// @return -2 if error, -1 if a token was matched but its of another type,
/// 0 if no token was match, 1 if a token was matched and its of the correct
/// type
int match_token_nodata(char** strp, token_t* tp, const char* const* types, int num_words, int enum_offset);

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

// ===================== MACROS =====================

#define IS_KEYWORD(strp, tp) match_token_nodata((strp), (tp), keywords, sizeof(keywords) / sizeof(keywords[0]), K_VOID)

#define IS_ASSIGNMENT_OPERATOR(strp, tp)                   \
    match_token_nodata((strp), (tp), assignment_operators, \
                       sizeof(assignment_operators) / sizeof(assignment_operators[0]), SO_SIMPLE)

#define IS_RELATIONAL_OPERATOR(strp, tp)                   \
    match_token_nodata((strp), (tp), relational_operators, \
                       sizeof(relational_operators) / sizeof(relational_operators[0]), RO_EQ)

#define IS_LOGIC_OPERATOR(strp, tp) \
    match_token_nodata((strp), (tp), logic_operators, sizeof(logic_operators) / sizeof(logic_operators[0]), LO_NOT)

#define IS_BITWISE_OPERATOR(strp, tp)                                                                             \
    match_token_nodata((strp), (tp), bitwise_operators, sizeof(bitwise_operators) / sizeof(bitwise_operators[0]), \
                       BW_NOT)

#define IS_ARITHMETIC_OPERATOR(strp, tp)                   \
    match_token_nodata((strp), (tp), arithmetic_operators, \
                       sizeof(arithmetic_operators) / sizeof(arithmetic_operators[0]), AO_SUM)

#define IS_SEPARATOR(strp, tp) \
    match_token_nodata((strp), (tp), separators, sizeof(separators) / sizeof(separators[0]), S_SQ)

// ===================== WRAPPER FUNCTIONS =====================

int is_keyword(char** strp, token_t* tp);
int is_assignment_operator(char** strp, token_t* tp);
int is_relational_operator(char** strp, token_t* tp);
int is_logic_operator(char** strp, token_t* tp);
int is_bitwise_operator(char** strp, token_t* tp);
int is_arithmetic_operator(char** strp, token_t* tp);
int is_separator(char** strp, token_t* tp);

// =========== MATCH DATA ==============
int is_char_literal(char** strp, token_t* tp);
int is_integer_literal(char** strp, token_t* tp);
int is_string_literal(char** strp, token_t* tp);
int is_identifier(char** strp, token_t* tp);

#endif
