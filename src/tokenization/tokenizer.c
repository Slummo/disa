#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "utils/str.h"
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define PARTIAL_SIZE 256
#define CHUNK_SIZE 4096
#define ID_MAX 256

// ===================== MATCH_NODATA =====================

const char* match_to_string(match_t m) {
    switch (m) {
        case MATCH_ERR:
            return "MATCH_ERR";
        case MATCH_NONE:
            return "MATCH_NONE";
        case MATCH_PARTIAL:
            return "MATCH_PARTIAL";
        case MATCH_FULL:
            return "MATCH_FULL";
        case MATCH_FULL_DIFF:
            return "MATCH_FULL_DIFF";
        default:
            return "UNKNOWN";
    }
}

typedef match_t (*match_action_fn)(char** strp, token_t* tp, int index, size_t token_len);

typedef struct {
    const char* const* symbols;
    int nsymbols;
    match_action_fn action;
    int instant_match;
} match_data_t;

static match_t match_from_data(char** strp, token_t* tp, const match_data_t* data) {
    if (!strp || !*strp || !tp) {
        return MATCH_ERR;
    }
    skip_spaces(strp);
    const char* str = *strp;

    for (int i = 0; i < data->nsymbols; i++) {
        const char* symbol = data->symbols[i];
        size_t token_len = strlen(symbol);
        size_t input_len = strlen(str);

        if (input_len < token_len) {
            if (!strncmp(str, symbol, input_len)) {
                return MATCH_PARTIAL;
            }

            continue;
        }

        // If the strings match and if data->instant_match is true or the next char is alnum or underscore
        if (!strncmp(str, symbol, token_len) &&
            (data->instant_match || !(isalnum(str[token_len]) || str[token_len] == '_'))) {
            return data->action(strp, tp, i, token_len);
        }
    }

    return MATCH_NONE;
}

static match_t keyword_action(char** strp, token_t* tp, int i, size_t token_len) {
    *tp = token_new((token_type_t)(i + K_VOID));
    str_advance(strp, token_len);
    skip_spaces(strp);
    return MATCH_FULL;
}

static const match_data_t keyword_data = {keywords, sizeof(keywords) / sizeof(keywords[0]), keyword_action, 0};

match_t match_keyword(char** strp, token_t* tp) {
    return match_from_data(strp, tp, &keyword_data);
}

static match_t assignment_action(char** strp, token_t* tp, int i, size_t token_len) {
    *tp = token_new((token_type_t)(i + SO_SIMPLE));
    str_advance(strp, token_len);
    skip_spaces(strp);
    return MATCH_FULL;
}

static const match_data_t assignment_data = {
    assignment_operators, sizeof(assignment_operators) / sizeof(assignment_operators[0]), assignment_action, 1};

match_t match_assignment_operator(char** strp, token_t* tp) {
    return match_from_data(strp, tp, &assignment_data);
}

static match_t relational_action(char** strp, token_t* tp, int i, size_t token_len) {
    // If a > or < is actually a >= or <=
    if ((relational_operators[i][0] == '<' || relational_operators[i][0] == '>') && (*strp)[token_len] == '=') {
        *tp = token_new(relational_operators[i][0] == '<' ? RO_LE : RO_GE);
        str_advance(strp, token_len + 1);
        skip_spaces(strp);
        return MATCH_FULL;
    }

    *tp = token_new((token_type_t)(i + RO_EQ));
    str_advance(strp, token_len);
    skip_spaces(strp);
    return MATCH_FULL;
}

static const match_data_t relational_data = {
    relational_operators, sizeof(relational_operators) / sizeof(relational_operators[0]), relational_action, 1};

match_t match_relational_operator(char** strp, token_t* tp) {
    return match_from_data(strp, tp, &relational_data);
}

static match_t logic_action(char** strp, token_t* tp, int i, size_t token_len) {
    // If a ! is actually the != relational operator
    if (logic_operators[i][0] == '!' && (*strp)[token_len] == '=') {
        *tp = token_new(RO_NEQ);
        str_advance(strp, token_len + 1);
        skip_spaces(strp);
        return MATCH_FULL_DIFF;
    }

    *tp = token_new((token_type_t)(i + LO_NOT));
    str_advance(strp, token_len);
    skip_spaces(strp);
    return MATCH_FULL;
}

static const match_data_t logic_data = {logic_operators, sizeof(logic_operators) / sizeof(logic_operators[0]),
                                        logic_action, 1};

match_t match_logic_operator(char** strp, token_t* tp) {
    return match_from_data(strp, tp, &logic_data);
}

static match_t bitwise_action(char** strp, token_t* tp, int i, size_t token_len) {
    // If it's actually an assignment operator
    if ((*strp)[token_len] == '=' && bitwise_operators[i][0] != '~') {
        *tp = token_new((token_type_t)(i + SO_SIMPLE + 5));
        str_advance(strp, token_len + 1);
        skip_spaces(strp);
        return MATCH_FULL_DIFF;
    }

    *tp = token_new((token_type_t)(i + BW_NOT));
    str_advance(strp, token_len);
    skip_spaces(strp);
    return MATCH_FULL;
}

static const match_data_t bitwise_data = {bitwise_operators, sizeof(bitwise_operators) / sizeof(bitwise_operators[0]),
                                          bitwise_action, 1};

match_t match_bitwise_operator(char** strp, token_t* tp) {
    return match_from_data(strp, tp, &bitwise_data);
}

static match_t arithmetic_action(char** strp, token_t* tp, int i, size_t token_len) {
    // If it's actually an assignment operator
    if ((*strp)[token_len] == '=') {
        *tp = token_new((token_type_t)(i + SO_SIMPLE + 1));
        str_advance(strp, token_len + 1);
        skip_spaces(strp);
        return MATCH_FULL_DIFF;
    }

    *tp = token_new((token_type_t)(i + AO_SUM));
    str_advance(strp, token_len);
    skip_spaces(strp);
    return MATCH_FULL;
}

static const match_data_t arithmetic_data = {
    arithmetic_operators, sizeof(arithmetic_operators) / sizeof(arithmetic_operators[0]), arithmetic_action, 1};

match_t match_arithmetic_operator(char** strp, token_t* tp) {
    return match_from_data(strp, tp, &arithmetic_data);
}

static match_t separator_action(char** strp, token_t* tp, int i, size_t token_len) {
    *tp = token_new((token_type_t)(i + S_SQ));
    str_advance(strp, token_len);
    skip_spaces(strp);
    return MATCH_FULL;
}

static const match_data_t separator_data = {separators, sizeof(separators) / sizeof(separators[0]), separator_action,
                                            1};

match_t match_separator(char** strp, token_t* tp) {
    return match_from_data(strp, tp, &separator_data);
}

// ===================== MATCH DATA =======================

match_t match_char_literal(char** strp, token_t* tp) {
    if (!strp || !*strp || !tp) {
        return MATCH_ERR;
    }
    skip_spaces(strp);

    char* str = *strp;

    if (*str != '\'') {
        return MATCH_NONE;
    }
    str++;

    char c;
    if (*str == '\\') {
        // Skip escape char
        str++;

        if (!*str) {
            return MATCH_PARTIAL;
        }

        switch (*str) {
            case 'n': {
                c = '\n';
                break;
            }
            case 't': {
                c = '\t';
                break;
            }
            case '\\': {
                c = '\\';
                break;
            }
            case '\'': {
                c = '\'';
                break;
            }
            case '"': {
                c = '\"';
                break;
            }
            // TODO: add more escape sequences
            default: {
                return MATCH_ERR;
            }
        }
    } else {
        // If there wasn't an escape char, then the
        // literal single quote is invalid
        if (*str == '\'') {
            return MATCH_ERR;
        }

        if (!*str) {
            return MATCH_PARTIAL;
        }

        c = *str;
    }
    str++;

    if (!*str) {
        return MATCH_PARTIAL;
    }

    if (*str != '\'') {
        return MATCH_NONE;
    }
    str++;

    *tp = token_new_char(c);
    *strp = str;
    skip_spaces(strp);
    return MATCH_FULL;
}

match_t match_integer_literal(char** strp, token_t* tp) {
    if (!strp || !*strp || !tp) {
        return MATCH_ERR;
    }
    skip_spaces(strp);

    char* str = *strp;

    if (!isdigit(*str)) {
        return MATCH_NONE;
    }
    str++;

    // Skip over all the digits
    while (*str && isdigit(*str)) {
        str++;
    }

    if (isalpha(*str)) {
        return MATCH_NONE;
    }

    if (!*str) {
        return MATCH_PARTIAL;
    }

    char* end;
    errno = 0;
    long long res = strtoll(*strp, &end, 10);

    if (errno == ERANGE) {
        perror("strtoll");
        return MATCH_NONE;
    }

    *tp = token_new_int((int64_t)res);
    *strp = str;
    skip_spaces(strp);

    return MATCH_FULL;
}

match_t match_string_literal(char** strp, token_t* tp) {
    if (!strp || !*strp || !tp) {
        return MATCH_ERR;
    }
    skip_spaces(strp);

    char* str = *strp;

    if (*str != '"') {
        return MATCH_NONE;
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
        return MATCH_PARTIAL;
    }

    *tp = token_new_string(*strp + 1);
    *strp = str;
    skip_spaces(strp);
    return MATCH_FULL;
}

match_t match_identifier(char** strp, token_t* tp) {
    if (!strp || !*strp || !tp) {
        return MATCH_NONE;
    }
    skip_spaces(strp);

    char* str = *strp;

    if (!isalpha(*str) && *str != '_') {
        return MATCH_NONE;
    }
    str++;

    int len = 1;
    while (*str && (isalnum(*str) || *str == '_')) {
        len++;
        str++;
    }

    if (!*str) {
        return MATCH_PARTIAL;
    }

    // Copy string to a buffer to add the terminator
    // char
    char buf[ID_MAX + 1] = {0};
    if (len > ID_MAX) {
        len = ID_MAX;
    }
    strncpy(buf, *strp, len);

    *tp = token_new_id(buf);
    *strp = str;
    skip_spaces(strp);
    return MATCH_FULL;
}

// ==================== TOKENIZER ========================

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
    memset(t->partial, 0, PARTIAL_SIZE);

    return t;
}

static match_t (*const matchers[])(char**, token_t*) = {match_keyword,
                                                        match_assignment_operator,
                                                        match_relational_operator,
                                                        match_logic_operator,
                                                        match_bitwise_operator,
                                                        match_arithmetic_operator,
                                                        match_char_literal,
                                                        match_integer_literal,
                                                        match_string_literal,
                                                        match_identifier,
                                                        match_separator};
static const int num_matchers = sizeof(matchers) / sizeof(*matchers);

static int process_buffer(char* buf, tokenizer_t t) {
    char* str = buf;
    while (*str) {
        int matched = 0;
        token_t tk = NULL;
        for (int i = 0; !matched && i < num_matchers; i++) {
            switch (matchers[i](&str, &tk)) {
                case MATCH_ERR: {
                    fprintf(stderr, "Tokenizer error at \"%s\"\n", str);
                    return 0;
                }
                case MATCH_NONE: {
                    break;
                }
                case MATCH_PARTIAL: {
                    matched = 1;

                    // Store the rest of the buffer for next chunk
                    snprintf(t->partial, PARTIAL_SIZE, "%s", str);
                    *str = '\0';
                    break;
                }
                case MATCH_FULL:
                case MATCH_FULL_DIFF: {
                    matched = 1;
                    tlist_append_token(&t->tokens, tk);
                    break;
                }
            }
        }

        if (!matched) {
            fprintf(stderr, "Warning: Unrecognized token starting with '%c'\n", *str);
            str++;
        }
    }
    return 1;
}

// Tokenizes the file (if found) with the tokenizer t
int tokenize(tokenizer_t t, const char* filename) {
    struct stat path_stat;
    if (stat(filename, &path_stat)) {
        perror("Error checking file");
        return 0;
    }

    if (S_ISDIR(path_stat.st_mode)) {
        fprintf(stderr, "Error: '%s' is a directory, not a file.\n", filename);
        return 0;
    }

    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Error opening file");
        return 0;
    }

    char chunk[CHUNK_SIZE + 1] = {0};
    char buf[PARTIAL_SIZE + CHUNK_SIZE + 1] = {0};

    size_t nread;
    while ((nread = fread(chunk, 1, CHUNK_SIZE, f)) > 0) {
        chunk[nread] = '\0';

        // Prepend partial data from last reading
        snprintf(buf, sizeof(buf), "%s%s", t->partial, chunk);
        t->partial[0] = '\0';

        if (!process_buffer(buf, t)) {
            fclose(f);
            return 0;
        }
    }

    // Check if there's something left in partial
    if (t->partial[0]) {
        fprintf(stderr, "Warning: leftover \"%s\"\n", t->partial);
    }

    fclose(f);
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