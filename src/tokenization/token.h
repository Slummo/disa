#ifndef TOKEN_H
#define TOKEN_H

typedef enum token_type {
    // Keywords
    K_VOID,      // void
    K_CHAR,      // char
    K_SHORT,     // short
    K_INT,       // int
    K_LONG,      // long
    K_RETURN,    // return
    K_IF,        // if
    K_ELSE,      // else
    K_WHILE,     // while
    K_FOR,       // for
    K_CONTINUE,  // continue
    K_BREAK,     // break
    K_SIGNED,    // signed
    K_UNSIGNED,  // unsigned

    // Assignment operators
    SO_SIMPLE,  // =
    SO_ADD,     // +=
    SO_SUB,     // -=
    SO_MUL,     // *=
    SO_DIV,     // /=
    SO_MOD,     // %=
    SO_OR,      //|=
    SO_AND,     //&=
    SO_XOR,     //^=
    SO_LSHIFT,  // <<=
    SO_RSHIFT,  //>>=

    // Relational operators
    RO_EQ,   // ==
    RO_NEQ,  // !=
    RO_LT,   // <
    RO_LE,   // <=
    RO_GT,   // >
    RO_GE,   // >=

    // Logic operators
    LO_NOT,  // !
    LO_OR,   // ||
    LO_AND,  // &&

    // Bitwise operators
    BW_NOT,     // ~
    BW_OR,      // |
    BW_AND,     // &
    BW_XOR,     // ^
    BW_LSFHIT,  // <<
    BW_RSHIFT,  // >>

    // Arithmetic operators
    AO_SUM,  // +
    AO_SUB,  // -
    AO_MUL,  // *
    AO_DIV,  // /
    AO_MOD,  // %

    // Separators
    S_SQ,    // '
    S_DQ,    // "
    S_QM,    // ?
    S_OP,    // (
    S_CP,    // )
    S_OS,    // [
    S_CS,    // ]
    S_OC,    // {
    S_CC,    // }
    S_COM,   // ,
    S_COL,   // :
    S_SCOL,  // ;

    // Literals
    L_C,  // e.g. 'x'
    L_I,  // e.g. 5
    L_S,  // e.g. "hello"

    // Identifiers
    ID  // e.g. x
} token_type_t;
typedef union token_value token_value_t;
typedef struct token _token, *token_t;

// Create a new token that doesn't carry additional data
token_t token_new(token_type_t type);

// Create a new token for a character literal
token_t token_new_char(char value);

// Create a new token for an integer literal
token_t token_new_int(int value);

// Create a new token for a string literal
token_t token_new_string(const char* value);

// Create a new token for a identifier
token_t token_new_id(const char* id);

token_t token_clone(const token_t t);

void token_print(const token_t t);

void token_free(token_t* tp);

#endif