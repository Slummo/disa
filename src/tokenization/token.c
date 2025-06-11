#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include "../utils/str.h"

union token_value {
    char cvalue;
    int ivalue;
    char* svalue;
};

struct token {
    token_type_t type;
    token_value_t value;
    int has_value;
    int needs_free;
};

// Create a new token that doesn't carry additional data.
token_t token_new(token_type_t type) {
    token_t t = (token_t)malloc(sizeof(_token));
    if (!t) {
        return NULL;
    }

    t->type = type;

    // Initialize to standard value
    t->value.cvalue = 0;

    t->has_value = 0;
    t->needs_free = 0;

    return t;
}

// Create a new token for a character literal.
token_t token_new_char(char value) {
    token_t t = (token_t)malloc(sizeof(_token));
    if (!t) {
        return NULL;
    }

    t->type = L_C;
    t->value.cvalue = value;
    t->has_value = 1;
    t->needs_free = 0;

    return t;
}

// Create a new token for an integer literal.
token_t token_new_int(int value) {
    token_t t = (token_t)malloc(sizeof(_token));
    if (!t) {
        return NULL;
    }

    t->type = L_I;
    t->value.ivalue = value;
    t->has_value = 1;
    t->needs_free = 0;

    return t;
}

// Create a new token for a string literal.
token_t token_new_string(const char* value) {
    token_t t = (token_t)malloc(sizeof(_token));
    if (!t) {
        return NULL;
    }

    t->type = L_S;
    t->value.svalue = strdup(value);
    t->has_value = 1;
    t->needs_free = 1;

    return t;
}

// Create a new token for a identifier.
token_t token_new_id(const char* id) {
    token_t t = (token_t)malloc(sizeof(_token));
    if (!t) {
        return NULL;
    }

    t->type = ID;
    t->value.svalue = strdup(id);
    t->has_value = 1;
    t->needs_free = 1;

    return t;
}

token_t token_clone(const token_t t) {
    if (!t) {
        return NULL;
    }

    // Create a new clone using predefined functions
    if (t->type == L_S) {
        return token_new_string(t->value.svalue);
    }

    if (t->type == ID) {
        return token_new_id(t->value.svalue);
    }

    // Create a new clone from scratch
    token_t clone = (token_t)malloc(sizeof(_token));
    if (!clone) {
        return NULL;
    }

    // Shallow copy
    *clone = *t;
    return clone;
}

void token_print(const token_t t) {
    if (!t) {
        printf("NULL");
        return;
    }

    switch (t->type) {
        case K_VOID: {
            printf("K_VOID");
            break;
        }
        case K_CHAR: {
            printf("K_CHAR");
            break;
        }
        case K_SHORT: {
            printf("K_SHORT");
            break;
        }
        case K_INT: {
            printf("K_INT");
            break;
        }
        case K_LONG: {
            printf("K_LONG");
            break;
        }
        case K_RETURN: {
            printf("K_RETURN");
            break;
        }
        case K_IF: {
            printf("K_IF");
            break;
        }
        case K_ELSE: {
            printf("K_ELSE");
            break;
        }
        case K_WHILE: {
            printf("K_WHILE");
            break;
        }
        case K_FOR: {
            printf("K_FOR");
            break;
        }
        case K_CONTINUE: {
            printf("K_CONTINUE");
            break;
        }
        case K_BREAK: {
            printf("K_BREAK");
            break;
        }
        case K_SIGNED: {
            printf("K_SIGNED");
            break;
        }
        case K_UNSIGNED: {
            printf("K_FOR");
            break;
        }

        case SO_SIMPLE: {
            printf("SO_SIMPLE");
            break;
        }
        case SO_ADD: {
            printf("SO_ADD");
            break;
        }
        case SO_SUB: {
            printf("SO_SUB");
            break;
        }
        case SO_MUL: {
            printf("SO_MUL");
            break;
        }
        case SO_DIV: {
            printf("SO_DIV");
            break;
        }
        case SO_MOD: {
            printf("SO_MOD");
            break;
        }
        case SO_OR: {
            printf("SO_OR");
            break;
        }
        case SO_AND: {
            printf("SO_AND");
            break;
        }
        case SO_XOR: {
            printf("SO_XOR");
            break;
        }
        case SO_LSHIFT: {
            printf("SO_LSHIFT");
            break;
        }
        case SO_RSHIFT: {
            printf("SO_RSHIFT");
            break;
        }

        case RO_EQ: {
            printf("RO_EQ");
            break;
        }
        case RO_GE: {
            printf("RO_GE");
            break;
        }
        case RO_GT: {
            printf("RO_GT");
            break;
        }
        case RO_LE: {
            printf("RO_LE");
            break;
        }
        case RO_LT: {
            printf("RO_LT");
            break;
        }
        case RO_NEQ: {
            printf("RO_NEQ");
            break;
        }

        case LO_NOT: {
            printf("LO_NOT");
            break;
        }
        case LO_OR: {
            printf("LO_OR");
            break;
        }
        case LO_AND: {
            printf("LO_AND");
            break;
        }

        case BW_NOT: {
            printf("BW_NOT");
            break;
        }
        case BW_OR: {
            printf("BW_OR");
            break;
        }
        case BW_AND: {
            printf("BW_AND");
            break;
        }
        case BW_XOR: {
            printf("BW_XOR");
            break;
        }
        case BW_LSFHIT: {
            printf("BW_LSFHIT");
            break;
        }
        case BW_RSHIFT: {
            printf("BW_RSHIFT");
            break;
        }

        case AO_SUM: {
            printf("AO_SUM");
            break;
        }
        case AO_SUB: {
            printf("AO_SUB");
            break;
        }
        case AO_MUL: {
            printf("AO_MUL");
            break;
        }
        case AO_DIV: {
            printf("AO_DIV");
            break;
        }
        case AO_MOD: {
            printf("AO_MOD");
            break;
        }

        case S_SQ: {
            printf("S_SQ");
            break;
        }
        case S_DQ: {
            printf("S_DQ");
            break;
        }
        case S_QM: {
            printf("S_QM");
            break;
        }
        case S_OP: {
            printf("S_OP");
            break;
        }
        case S_CP: {
            printf("S_CP");
            break;
        }
        case S_OS: {
            printf("S_OS");
            break;
        }
        case S_CS: {
            printf("S_CS");
            break;
        }
        case S_OC: {
            printf("S_OC");
            break;
        }
        case S_CC: {
            printf("S_CC");
            break;
        }
        case S_COM: {
            printf("S_COM");
            break;
        }
        case S_COL: {
            printf("S_COL");
            break;
        }
        case S_SCOL: {
            printf("S_SCOL");
            break;
        }

        case L_C: {
            printf("L_C(%c)", t->value.cvalue);
            break;
        }
        case L_I: {
            printf("L_I(%d)", t->value.ivalue);
            break;
        }
        case L_S: {
            printf("L_S(%s)", t->value.svalue);
            break;
        }
        case ID: {
            printf("ID(%s)", t->value.svalue);
            break;
        }
    }
}

void token_free(token_t* tp) {
    if (!tp || !(*tp)->needs_free || !(*tp)->has_value || !(*tp)->value.svalue) {
        return;
    }

    free((*tp)->value.svalue);
    free(*tp);
    *tp = NULL;
}