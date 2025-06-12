#include "tests.h"
#include "tokenization/token.h"
#include "utils/str.h"
#include <string.h>
#include <stdio.h>

void run_token_test(const char* label, int (*token_fn)(char**, token_t*), const char* str, int expected_result,
                    const char* expected_str, token_type_t expected_token_type, token_t* tp) {
    char buffer[256] = {0};
    char* str2 = NULL;

    if (str) {
        strncpy(buffer, str, sizeof(buffer));
        str2 = buffer;
    }

    int result = token_fn(&str2, tp);

    // Compare strings safely
    int str_matches = 0;
    if (!expected_str && !str2) {
        str_matches = 1;
    } else if (expected_str && str2) {
        str_matches = !strcmp(str2, expected_str);
    }

    int pass = (result == expected_result) && str_matches && (token_get_type(*tp) == expected_token_type);

    // Print aligned output
    printf("%s: %s\n\t-expected result: %d, got: %d\n\t-expected new str: '%s', got: '%s'\n\t-expected token "
           "type: '%s', got: %s\n",
           label, pass ? "✅ OK" : "❌ FAIL", expected_result, result, expected_str ? expected_str : "NULL",
           str2 ? str2 : "NULL", token_type_to_str(expected_token_type), token_type_to_str(token_get_type(*tp)));
}

void token_matching() {
    printf("=============Testing for token matching=============\n");

    char* str = NULL;
    token_t t = NULL;

    // ================== KEYWORDS ==================
    run_token_test("IS_KEYWORD(NULL)", is_keyword, str, -2, NULL, T_NOVALUE, &t);

    str = "   int";
    t = NULL;
    run_token_test("IS_KEYWORD('   int')", is_keyword, str, 1, "", K_INT, &t);

    str = "   int_";
    t = NULL;
    run_token_test("IS_KEYWORD('   int_')", is_keyword, str, 0, "int_", T_NOVALUE, &t);

    str = "   void   abc";
    t = NULL;
    run_token_test("IS_KEYWORD('   void   abc')", is_keyword, str, 1, "abc", K_VOID, &t);

    str = "   voidabc";
    t = NULL;
    run_token_test("IS_KEYWORD('   voidabc')", is_keyword, str, 0, "voidabc", T_NOVALUE, &t);

    // ================== ASSIGNMENT OPERATORS ==================
    str = " += next";
    t = NULL;
    run_token_test("IS_ASSIGNMENT_OPERATOR(' += next')", is_assignment_operator, str, 1, "next", SO_ADD, &t);

    str = " + next";
    t = NULL;
    run_token_test("IS_ASSIGNMENT_OPERATOR(' + next')", is_assignment_operator, str, 0, "+ next", T_NOVALUE, &t);

    str = " +=abc";
    t = NULL;
    run_token_test("IS_ASSIGNMENT_OPERATOR(' +=abc')", is_assignment_operator, str, 1, "abc", SO_ADD, &t);

    str = " *=x";
    t = NULL;
    run_token_test("IS_ASSIGNMENT_OPERATOR(' *=x')", is_assignment_operator, str, 1, "x", SO_MUL, &t);

    // ================== LOGIC OPERATORS ==================
    str = " &&x";
    t = NULL;
    run_token_test("IS_LOGIC_OPERATOR(' &&x')", is_logic_operator, str, 1, "x", LO_AND, &t);

    str = " &=x";
    t = NULL;
    run_token_test("IS_LOGIC_OPERATOR(' &=x')", is_logic_operator, str, 0, "&=x", T_NOVALUE, &t);

    str = " !x";
    t = NULL;
    run_token_test("IS_LOGIC_OPERATOR(' !x')", is_logic_operator, str, 1, "x", LO_NOT, &t);

    str = " !=x";
    t = NULL;
    run_token_test("IS_LOGIC_OPERATOR(' !=x')", is_logic_operator, str, -1, "x", RO_NEQ, &t);

    // ================== RELATIONAL OPERATORS ==================
    str = " ==   ";
    t = NULL;
    run_token_test("IS_RELATIONAL_OPERATOR(' ==   ')", is_relational_operator, str, 1, "", RO_EQ, &t);

    str = " >";
    t = NULL;
    run_token_test("IS_RELATIONAL_OPERATOR(' >')", is_relational_operator, str, 1, "", RO_GT, &t);

    str = " >=more";
    t = NULL;
    run_token_test("IS_RELATIONAL_OPERATOR(' >=more')", is_relational_operator, str, 1, "more", RO_GE, &t);

    str = " <= x abc";
    t = NULL;
    run_token_test("IS_RELATIONAL_OPERATOR(' <= x abc')", is_relational_operator, str, 1, "x abc", RO_LE, &t);

    // ================== BITWISE OPERATORS ==================
    str = " >>abc";
    t = NULL;
    run_token_test("IS_BITWISE_OPERATOR(' >>abc')", is_bitwise_operator, str, 1, "abc", BW_RSHIFT, &t);

    str = " <<=abc";
    t = NULL;
    run_token_test("IS_BITWISE_OPERATOR(' <<=abc')", is_bitwise_operator, str, -1, "abc", SO_LSHIFT, &t);

    str = " & next";
    t = NULL;
    run_token_test("IS_BITWISE_OPERATOR(' & next')", is_bitwise_operator, str, 1, "next", BW_AND, &t);

    str = " |= next";
    t = NULL;
    run_token_test("IS_BITWISE_OPERATOR(' |= next')", is_bitwise_operator, str, -1, "next", SO_OR, &t);

    str = " ~= next";
    t = NULL;
    run_token_test("IS_BITWISE_OPERATOR(' ~= next')", is_bitwise_operator, str, 1, "= next", BW_NOT, &t);

    str = " ~next";
    t = NULL;
    run_token_test("IS_BITWISE_OPERATOR(' ~next')", is_bitwise_operator, str, 1, "next", BW_NOT, &t);

    // ================== ARITHMETIC OPERATORS ==================
    str = " + x";
    t = NULL;
    run_token_test("IS_ARITHMETIC_OPERATOR(' + x')", is_arithmetic_operator, str, 1, "x", AO_SUM, &t);

    str = " += x";
    t = NULL;
    run_token_test("IS_ARITHMETIC_OPERATOR(' += x')", is_arithmetic_operator, str, -1, "x", SO_ADD, &t);

    str = " *= x";
    t = NULL;
    run_token_test("IS_ARITHMETIC_OPERATOR(' *= x')", is_arithmetic_operator, str, -1, "x", SO_MUL, &t);

    // ================== SEPARATORS ==================
    str = " { int x; }";
    t = NULL;
    run_token_test("IS_SEPARATOR(' { int x; }')", is_separator, str, 1, "int x; }", S_OC, &t);

    str = " ; end";
    t = NULL;
    run_token_test("IS_SEPARATOR(' ; end')", is_separator, str, 1, "end", S_SCOL, &t);

    str = " ? next";
    t = NULL;
    run_token_test("IS_SEPARATOR(' ? next')", is_separator, str, 1, "next", S_QM, &t);

    // ================== LITERALS ==================
    str = "  'x'";
    t = NULL;
    printf("is_char_literal('  'x'') | expected: 1, got: %d. Token: ", is_char_literal(&str, &t));
    token_print(t);
    puts("");

    str = "  x'";
    t = NULL;
    printf("is_char_literal('  x'') | expected: 0, got: %d. Token: ", is_char_literal(&str, &t));
    token_print(t);
    puts("");

    str = "  'x";
    t = NULL;
    printf("is_char_literal('  'x') | expected: 0, got: %d. Token: ", is_char_literal(&str, &t));
    token_print(t);
    puts("");

    char buf[] = "  ciao = 4";
    str = buf;
    t = NULL;
    printf("is_string_literal('  ciao = 4') | expected: 0, got: %d. Token: ", is_string_literal(&str, &t));
    token_print(t);
    puts("");

    char buf2[] = "  \"ciao   \"";
    str = buf2;
    t = NULL;
    printf("is_string_literal('  \"ciao   \"') | expected: 1, got: %d. Token: ", is_string_literal(&str, &t));
    token_print(t);
    puts("");

    char buf3[] = "  ciao   \"";
    str = buf3;
    t = NULL;
    printf("is_string_literal('  ciao   \"') | expected: 0, got: %d. Token: ", is_string_literal(&str, &t));
    token_print(t);
    puts("");

    char buf4[] = "  \"ciao   ";
    str = buf4;
    t = NULL;
    printf("is_string_literal('  \"ciao   ') | expected: 0, got: %d. Token: ", is_string_literal(&str, &t));
    token_print(t);
    puts("");

    str = "  a5";
    t = NULL;
    printf("is_integer_literal('  a5') | expected: 0, got: %d. Token: ", is_integer_literal(&str, &t));
    token_print(t);
    puts("");

    str = "  3192384";
    t = NULL;
    printf("is_integer_literal('  3192384') | expected: 1, got: %d. Token: ", is_integer_literal(&str, &t));
    token_print(t);
    puts("");

    str = "  100;";
    t = NULL;
    printf("is_integer_literal('  100;') | expected: 1, got: %d. Token: ", is_integer_literal(&str, &t));
    token_print(t);
    puts("");

    str = "  100 ";
    t = NULL;
    printf("is_integer_literal('  100 ') | expected: 1, got: %d. Token: ", is_integer_literal(&str, &t));
    token_print(t);
    puts("");

    str = "  100+";
    t = NULL;
    printf("is_integer_literal('  100+') | expected: 1, got: %d. Token: ", is_integer_literal(&str, &t));
    token_print(t);
    puts("");

    str = "  abc";
    t = NULL;
    printf("is_identifier('  abc') | expected: 1, got: %d. Token: ", is_identifier(&str, &t));
    token_print(t);
    puts("");

    str = "  x12_ ";
    t = NULL;
    printf("is_identifier('  x12_') | expected: 1, got: %d. Token: ", is_identifier(&str, &t));
    token_print(t);
    puts("");

    str = "  = 5";
    t = NULL;
    printf("is_identifier('  = 5') | expected: 0, got: %d. Token: ", is_identifier(&str, &t));
    token_print(t);
    puts("");

    str = "  100+";
    t = NULL;
    printf("is_identifier('  100+') | expected: 0, got: %d. Token: ", is_identifier(&str, &t));
    token_print(t);
    puts("");

    str = "  _45";
    t = NULL;
    printf("is_identifier('  _45') | expected: 1, got: %d. Token: ", is_identifier(&str, &t));
    token_print(t);
    puts("");

    str = "int_5";
    t = NULL;
    printf("is_identifier('int_5') | expected: 1, got: %d. Token: ", is_identifier(&str, &t));
    token_print(t);
    puts("");
}