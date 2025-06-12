#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

void skip_spaces(char** strp) {
    if (!strp || !*strp) {
        return;
    }

    while (**strp && isspace(**strp)) {
        (*strp)++;
    }
}

// Advances a char* pointer without making checks
void str_advance(char** strp, int amount) {
    if (!strp || !*strp) {
        return;
    }

    (*strp) += amount;
}

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
int match_token_nodata(char** strp, token_t* tp, const char* const* types, int types_len, int enum_offset) {
    if (!strp || !*strp || !tp) {
        return -2;
    }
    skip_spaces(strp);

    for (int i = 0; i < types_len; i++) {
        size_t len = strlen(types[i]);

        // If the strings don't match or if it's not an operator and part of an identifier
        if (strncmp((*strp), types[i], len) ||
            ((enum_offset < SO_SIMPLE || enum_offset >= S_SQ) && (isalnum((*strp)[len]) || (*strp)[len] == '_'))) {
            continue;
        }

        // If an arithmetic/bitwise operator is actually an assignment operator
        if (enum_offset >= BW_NOT && enum_offset < S_SQ && (*strp)[len] == '=' && types[i][0] != '~') {
            // Adjust offset because of enum layout (+5 bitwise, +1 arithmetic)
            int offset = i + SO_SIMPLE;
            offset += enum_offset < AO_SUM ? 5 : 1;

            *tp = token_new((token_type_t)offset);
            str_advance(strp, len + 1);
            skip_spaces(strp);
            return -1;
        }

        // If a ! logic operator is actually the != relational operator
        if (types[i][0] == '!' && (*strp)[len] == '=') {
            *tp = token_new(RO_NEQ);
            str_advance(strp, len + 1);
            skip_spaces(strp);
            return -1;
        }

        // If a > or < relational operator is actually a >= or <= relational operator
        if ((types[i][0] == '<' || types[i][0] == '>') && (*strp)[len] == '=') {
            *tp = token_new(types[i][0] == '<' ? RO_LE : RO_GE);
            str_advance(strp, len + 1);
            skip_spaces(strp);
            return 1;
        }

        *tp = token_new((token_type_t)(i + enum_offset));
        str_advance(strp, len);
        skip_spaces(strp);

        return 1;
    }

    return 0;
}

int is_keyword(char** strp, token_t* tp) {
    return IS_KEYWORD(strp, tp);
}
int is_assignment_operator(char** strp, token_t* tp) {
    return IS_ASSIGNMENT_OPERATOR(strp, tp);
}
int is_relational_operator(char** strp, token_t* tp) {
    return IS_RELATIONAL_OPERATOR(strp, tp);
}
int is_logic_operator(char** strp, token_t* tp) {
    return IS_LOGIC_OPERATOR(strp, tp);
}
int is_bitwise_operator(char** strp, token_t* tp) {
    return IS_BITWISE_OPERATOR(strp, tp);
}
int is_arithmetic_operator(char** strp, token_t* tp) {
    return IS_ARITHMETIC_OPERATOR(strp, tp);
}
int is_separator(char** strp, token_t* tp) {
    return IS_SEPARATOR(strp, tp);
}

int is_char_literal(char** strp, token_t* tp) {
    if (!strp || !*strp || !tp) {
        return -1;
    }
    skip_spaces(strp);

    if (strlen(*strp) >= 3 && (*strp)[0] == '\'' && (*strp)[2] == '\'') {
        *tp = token_new_char((*strp)[1]);

        *strp += 3;
        skip_spaces(strp);

        return 1;
    }

    return 0;
}

int is_integer_literal(char** strp, token_t* tp) {
    if (!strp || !*strp || !tp) {
        return -1;
    }
    skip_spaces(strp);

    char* str = *strp;

    if (!isdigit(*str)) {
        return 0;
    }
    str++;

    while (*str && isdigit(*str)) {
        str++;
    }

    if (isalpha(*str)) {
        return 0;
    }

    char* end;
    errno = 0;
    long res = strtol(*strp, &end, 10);

    if (errno == ERANGE) {
        perror("strtol");
        return 0;
    }

    *tp = token_new_int((int)res);
    *strp = str;
    skip_spaces(strp);

    return 1;
}

int is_string_literal(char** strp, token_t* tp) {
    if (!strp || !*strp || !tp) {
        return -1;
    }
    skip_spaces(strp);

    char* str = *strp;

    if (*str != '"') {
        return 0;
    }
    str++;

    int end_found = 0;
    while (*str && !end_found) {
        if (*str == '"') {
            end_found = 1;

            // Substitute end of literal with terminator
            // to correctly create the new token
            *str = '\0';
        }

        str++;
    }

    if (!end_found) {
        return 0;
    }

    *tp = token_new_string(*strp + 1);
    *strp = str;
    skip_spaces(strp);
    return 1;
}

int is_identifier(char** strp, token_t* tp) {
    if (!strp || !*strp || !tp) {
        return -1;
    }
    skip_spaces(strp);

    char* str = *strp;

    if (!isalpha(*str) && *str != '_') {
        return 0;
    }
    str++;

    int len = 1;
    while (*str && (isalnum(*str) || *str == '_')) {
        len++;
        str++;
    }

    char buf[256];
    strncpy(buf, *strp, len);
    buf[len] = '\0';

    *tp = token_new_id(buf);
    *strp = str;
    skip_spaces(strp);
    return 1;
}