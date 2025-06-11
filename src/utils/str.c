#include "str.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Duplicates a string by calling malloc
// and strcpy
char* strdup(const char* str) {
    if (!str) {
        printf("Error: pointer str is NULL\n");
        return NULL;
    }

    size_t len = strlen(str) + 1;
    char* duplicate = (char*)malloc(sizeof(char) * len);
    if (duplicate) {
        strncpy(duplicate, str, len);
    } else {
        perror("Error with malloc");
    }

    return duplicate;
}

void skip_spaces(char** strp) {
    if (!strp || !*strp) {
        return;
    }

    while (**strp && isspace(**strp)) {
        (*strp)++;
    }
}

// Checks if the char at index pos of str
// is equal to c.
// Does not perform bound checks on str
int str_c_equals(char* str, int pos, char c) {
    if (!str) {
        return -1;
    }

    return str[pos] && str[pos] == c;
}

// Advances a char* pointer without making checks
void str_advance(char** strp, int amount) {
    if (!strp || !*strp) {
        return;
    }

    (*strp) += amount;
}

// Checks if a string is part of an identifier.
// Does not perform bound checks on str
int is_str_id(char* str, int pos) {
    if (!str) {
        return -1;
    }

    return str[pos] && (isalnum(str[pos]) || str[pos] == '_');
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
            ((enum_offset < SO_SIMPLE || enum_offset >= S_SQ) && is_str_id(*strp, len))) {
            continue;
        }

        // If an arithmetic/bitwise operator is actually an assignment operator
        if (enum_offset >= BW_NOT && enum_offset < S_SQ && str_c_equals(*strp, len, '=') && types[i][0] != '~') {
            // Adjust offset because of enum layout (+5 bitwise, +1 arithmetic)
            int offset = i + SO_SIMPLE;
            offset += enum_offset < AO_SUM ? 5 : 1;

            *tp = token_new((token_type_t)offset);
            str_advance(strp, len + 1);
            skip_spaces(strp);
            return -1;
        }

        // If a ! logic operator is actually the != relational operator
        if (types[i][0] == '!' && str_c_equals(*strp, len, '=')) {
            *tp = token_new(RO_NEQ);
            str_advance(strp, len + 1);
            skip_spaces(strp);
            return -1;
        }

        // If a > or < relational operator is actually a >= or <= relational operator
        if ((types[i][0] == '<' || types[i][0] == '>') && str_c_equals(*strp, len, '=')) {
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