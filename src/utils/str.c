#include "str.h"
#include <ctype.h>

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