#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/str.h"

const char* token_type_to_str(token_type_t tt) {
    switch (tt) {
        case K_VOID:
            return "K_VOID";
        case K_CHAR:
            return "K_CHAR";
        case K_SHORT:
            return "K_SHORT";
        case K_INT:
            return "K_INT";
        case K_LONG:
            return "K_LONG";
        case K_RETURN:
            return "K_RETURN";
        case K_IF:
            return "K_IF";
        case K_ELSE:
            return "K_ELSE";
        case K_WHILE:
            return "K_WHILE";
        case K_FOR:
            return "K_FOR";
        case K_CONTINUE:
            return "K_CONTINUE";
        case K_BREAK:
            return "K_BREAK";
        case K_SIGNED:
            return "K_SIGNED";
        case K_UNSIGNED:
            return "K_UNSIGNED";

        case SO_SIMPLE:
            return "SO_SIMPLE";
        case SO_ADD:
            return "SO_ADD";
        case SO_SUB:
            return "SO_SUB";
        case SO_MUL:
            return "SO_MUL";
        case SO_DIV:
            return "SO_DIV";
        case SO_MOD:
            return "SO_MOD";
        case SO_OR:
            return "SO_OR";
        case SO_AND:
            return "SO_AND";
        case SO_XOR:
            return "SO_XOR";
        case SO_LSHIFT:
            return "SO_LSHIFT";
        case SO_RSHIFT:
            return "SO_RSHIFT";

        case RO_EQ:
            return "RO_EQ";
        case RO_GE:
            return "RO_GE";
        case RO_GT:
            return "RO_GT";
        case RO_LE:
            return "RO_LE";
        case RO_LT:
            return "RO_LT";
        case RO_NEQ:
            return "RO_NEQ";

        case LO_NOT:
            return "LO_NOT";
        case LO_OR:
            return "LO_OR";
        case LO_AND:
            return "LO_AND";

        case BW_NOT:
            return "BW_NOT";
        case BW_OR:
            return "BW_OR";
        case BW_AND:
            return "BW_AND";
        case BW_XOR:
            return "BW_XOR";
        case BW_LSFHIT:
            return "BW_LSHIFT";
        case BW_RSHIFT:
            return "BW_RSHIFT";

        case AO_SUM:
            return "AO_SUM";
        case AO_SUB:
            return "AO_SUB";
        case AO_MUL:
            return "AO_MUL";
        case AO_DIV:
            return "AO_DIV";
        case AO_MOD:
            return "AO_MOD";

        case S_SQ:
            return "S_SQ";
        case S_DQ:
            return "S_DQ";
        case S_QM:
            return "S_QM";
        case S_OP:
            return "S_OP";
        case S_CP:
            return "S_CP";
        case S_OS:
            return "S_OS";
        case S_CS:
            return "S_CS";
        case S_OC:
            return "S_OC";
        case S_CC:
            return "S_CC";
        case S_COM:
            return "S_COM";
        case S_COL:
            return "S_COL";
        case S_SCOL:
            return "S_SCOL";

        case L_C:
            return "L_C";
        case L_I:
            return "L_I";
        case L_S:
            return "L_S";
        case ID:
            return "ID";

        case T_NOVALUE:
            return "T_NOVALUE";

        default:
            return "UNKNOWN";
    }
}

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

token_type_t token_get_type(token_t t) {
    if (!t) {
        return T_NOVALUE;
    }

    return t->type;
}

token_value_t token_get_value(token_t t) {
    if (!t) {
        token_value_t v;
        v.cvalue = 0;
        return v;
    }

    return t->value;
}

int token_has_value(token_t t) {
    if (!t) {
        return -1;
    }

    return t->has_value;
}

int token_needs_free(token_t t) {
    if (!t) {
        return -1;
    }

    return t->needs_free;
}

void token_print(const token_t t) {
    if (!t) {
        printf("T_NOVALUE");
        return;
    }

    if (t->has_value) {
        switch (t->type) {
            case L_C: {
                printf("%s(%c)", token_type_to_str(t->type), t->value.cvalue);
                break;
            }
            case L_I: {
                printf("%s(%d)", token_type_to_str(t->type), t->value.ivalue);
                break;
            }
            case L_S:
            case ID: {
                printf("%s(%s)", token_type_to_str(t->type), t->value.svalue);
                break;
            }
            default: {
                fprintf(stderr,
                        "Error: t->has_value is true but t->type is a type of token that doesn't carry a value\n");
            }
        }
        return;
    }

    printf("%s", token_type_to_str(t->type));
}

void token_free(token_t* tp) {
    if (!tp || !(*tp)->needs_free || !(*tp)->has_value || !(*tp)->value.svalue) {
        return;
    }

    free((*tp)->value.svalue);
    free(*tp);
    *tp = NULL;
}