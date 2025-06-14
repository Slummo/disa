#include "tests.h"
#include <string.h>
#include <stdio.h>
#include "tokenization/tokenizer.h"

void run_token_test(const char* label, match_t (*token_fn)(char**, token_t*), const char* str, match_t expected_result,
                    const char* expected_str, token_type_t expected_token_type, token_t* tp) {
    char buffer[256];
    char* str2 = NULL;

    if (str) {
        strncpy(buffer, str, 255);
        buffer[255] = '\0';
        str2 = buffer;
    }

    match_t result = token_fn(&str2, tp);

    // Compare strings safely
    int str_matches = 0;
    if (!expected_str && !str2) {
        str_matches = 1;
    } else if (expected_str && str2) {
        str_matches = !strcmp(str2, expected_str);
    }

    int pass = (result == expected_result) && str_matches && (token_get_type(*tp) == expected_token_type);

    // Print aligned output
    printf("%s: %s\n\t-expected result: %s, got: %s\n\t-expected new str: \"%s\", got: \"%s\"\n\t-expected token "
           "type: %s, got: %s\n",
           label, pass ? "✅ OK" : "❌ FAIL", match_to_string(expected_result), match_to_string(result),
           expected_str ? expected_str : "NULL", str2 ? str2 : "NULL", token_type_to_str(expected_token_type),
           token_type_to_str(token_get_type(*tp)));
}

void token_matching() {
    printf("======================= Testing for token matching ========================\n");

    char* str = NULL;
    token_t t = NULL;

    // ================== KEYWORDS ==================
    run_token_test("match_keyword(NULL)", match_keyword, str, MATCH_ERR, NULL, T_NOVALUE, &t);

    str = "   int";
    t = NULL;
    run_token_test("match_keyword('   int')", match_keyword, str, MATCH_FULL, "", K_INT, &t);

    str = "   int_";
    t = NULL;
    run_token_test("match_keyword('   int_')", match_keyword, str, MATCH_NONE, "int_", T_NOVALUE, &t);

    str = "   void   abc";
    t = NULL;
    run_token_test("match_keyword('   void   abc')", match_keyword, str, MATCH_FULL, "abc", K_VOID, &t);

    str = "   voidabc";
    t = NULL;
    run_token_test("match_keyword('   voidabc')", match_keyword, str, MATCH_NONE, "voidabc", T_NOVALUE, &t);

    str = "   ret";
    t = NULL;
    run_token_test("match_keyword('   ret')", match_keyword, str, MATCH_PARTIAL, "ret", T_NOVALUE, &t);

    str = "w";
    t = NULL;
    run_token_test("match_keyword('w')", match_keyword, str, MATCH_PARTIAL, "w", T_NOVALUE, &t);

    // ================== ASSIGNMENT OPERATORS ==================
    str = " += next";
    t = NULL;
    run_token_test("match_assignment_operator(' += next')", match_assignment_operator, str, MATCH_FULL, "next", SO_ADD,
                   &t);

    str = " + next";
    t = NULL;
    run_token_test("match_assignment_operator(' + next')", match_assignment_operator, str, MATCH_NONE, "+ next",
                   T_NOVALUE, &t);

    str = " +=abc";
    t = NULL;
    run_token_test("match_assignment_operator(' +=abc')", match_assignment_operator, str, MATCH_FULL, "abc", SO_ADD,
                   &t);

    str = " *=x";
    t = NULL;
    run_token_test("match_assignment_operator(' *=x')", match_assignment_operator, str, MATCH_FULL, "x", SO_MUL, &t);

    // ================== LOGIC OPERATORS ==================
    str = " &&x";
    t = NULL;
    run_token_test("match_logic_operator(' &&x')", match_logic_operator, str, MATCH_FULL, "x", LO_AND, &t);

    str = " &=x";
    t = NULL;
    run_token_test("match_logic_operator(' &=x')", match_logic_operator, str, MATCH_NONE, "&=x", T_NOVALUE, &t);

    str = " !x";
    t = NULL;
    run_token_test("match_logic_operator(' !x')", match_logic_operator, str, MATCH_FULL, "x", LO_NOT, &t);

    str = " !=x";
    t = NULL;
    run_token_test("match_logic_operator(' !=x')", match_logic_operator, str, MATCH_FULL_DIFF, "x", RO_NEQ, &t);

    // ================== RELATIONAL OPERATORS ==================
    str = " ==   ";
    t = NULL;
    run_token_test("match_relational_operator(' ==   ')", match_relational_operator, str, MATCH_FULL, "", RO_EQ, &t);

    str = " >";
    t = NULL;
    run_token_test("match_relational_operator(' >')", match_relational_operator, str, MATCH_FULL, "", RO_GT, &t);

    str = " >=more";
    t = NULL;
    run_token_test("match_relational_operator(' >=more')", match_relational_operator, str, MATCH_FULL, "more", RO_GE,
                   &t);

    str = " <= x abc";
    t = NULL;
    run_token_test("match_relational_operator(' <= x abc')", match_relational_operator, str, MATCH_FULL, "x abc", RO_LE,
                   &t);

    str = " >x abc";
    t = NULL;
    run_token_test("match_relational_operator(' >x abc')", match_relational_operator, str, MATCH_FULL, "x abc", RO_GT,
                   &t);

    // ================== BITWISE OPERATORS ==================
    str = " >>abc";
    t = NULL;
    run_token_test("match_bitwise_operator(' >>abc')", match_bitwise_operator, str, MATCH_FULL, "abc", BW_RSHIFT, &t);

    str = " <<=abc";
    t = NULL;
    run_token_test("match_bitwise_operator(' <<=abc')", match_bitwise_operator, str, MATCH_FULL_DIFF, "abc", SO_LSHIFT,
                   &t);

    str = " & next";
    t = NULL;
    run_token_test("match_bitwise_operator(' & next')", match_bitwise_operator, str, MATCH_FULL, "next", BW_AND, &t);

    str = " |= next";
    t = NULL;
    run_token_test("match_bitwise_operator(' |= next')", match_bitwise_operator, str, MATCH_FULL_DIFF, "next", SO_OR,
                   &t);

    str = " ~= next";
    t = NULL;
    run_token_test("match_bitwise_operator(' ~= next')", match_bitwise_operator, str, MATCH_FULL, "= next", BW_NOT, &t);

    str = " ~next";
    t = NULL;
    run_token_test("match_bitwise_operator(' ~next')", match_bitwise_operator, str, MATCH_FULL, "next", BW_NOT, &t);

    // ================== ARITHMETIC OPERATORS ==================
    str = " + x";
    t = NULL;
    run_token_test("match_arithmetic_operator(' + x')", match_arithmetic_operator, str, MATCH_FULL, "x", AO_SUM, &t);

    str = " += x";
    t = NULL;
    run_token_test("match_arithmetic_operator(' += x')", match_arithmetic_operator, str, MATCH_FULL_DIFF, "x", SO_ADD,
                   &t);

    str = " *= x";
    t = NULL;
    run_token_test("match_arithmetic_operator(' *= x')", match_arithmetic_operator, str, MATCH_FULL_DIFF, "x", SO_MUL,
                   &t);

    str = " -x";
    t = NULL;
    run_token_test("match_arithmetic_operator(' -x')", match_arithmetic_operator, str, MATCH_FULL, "x", AO_SUB, &t);

    // ================== SEPARATORS ==================
    str = " { int x; }";
    t = NULL;
    run_token_test("match_separator(' { int x; }')", match_separator, str, MATCH_FULL, "int x; }", S_OC, &t);

    str = " ; end";
    t = NULL;
    run_token_test("match_separator(' ; end')", match_separator, str, MATCH_FULL, "end", S_SCOL, &t);

    str = " ? next";
    t = NULL;
    run_token_test("match_separator(' ? next')", match_separator, str, MATCH_FULL, "next", S_QM, &t);

    str = " ;next";
    t = NULL;
    run_token_test("match_separator(' ;next')", match_separator, str, MATCH_FULL, "next", S_SCOL, &t);

    str = " (next)";
    t = NULL;
    run_token_test("match_separator(' (next)')", match_separator, str, MATCH_FULL, "next)", S_OP, &t);

    // ================== LITERALS ==================
    str = "  'x'";
    t = NULL;
    run_token_test("match_char_literal(  'x')", match_char_literal, str, MATCH_FULL, "", L_C, &t);

    str = "  x'";
    t = NULL;
    run_token_test("match_char_literal(  x')", match_char_literal, str, MATCH_NONE, "x'", T_NOVALUE, &t);

    str = "  'x";
    t = NULL;
    run_token_test("match_char_literal(  'x)", match_char_literal, str, MATCH_PARTIAL, "'x", T_NOVALUE, &t);

    str = "  '\\n";
    t = NULL;
    run_token_test("match_char_literal(  '\\n)", match_char_literal, str, MATCH_PARTIAL, "'\\n", T_NOVALUE, &t);

    str = "  \\n'";
    t = NULL;
    run_token_test("match_char_literal(  \\n')", match_char_literal, str, MATCH_NONE, "\\n'", T_NOVALUE, &t);

    str = "  '\\n'";
    t = NULL;
    run_token_test("match_char_literal(  '\\n')", match_char_literal, str, MATCH_FULL, "", L_C, &t);

    str = "  '''";
    t = NULL;
    run_token_test("match_char_literal(  ''')", match_char_literal, str, MATCH_ERR, "'''", T_NOVALUE, &t);

    str = "  '";
    t = NULL;
    run_token_test("match_char_literal(  ')", match_char_literal, str, MATCH_PARTIAL, "'", T_NOVALUE, &t);

    str = "  '\\";
    t = NULL;
    run_token_test("match_char_literal(  '\\)", match_char_literal, str, MATCH_PARTIAL, "'\\", T_NOVALUE, &t);

    str = "  ''";
    t = NULL;
    run_token_test("match_char_literal(  '')", match_char_literal, str, MATCH_ERR, "''", T_NOVALUE, &t);

    char buf[] = "  ciao = 4";
    str = buf;
    t = NULL;
    run_token_test("match_string_literal(  ciao = 4)", match_string_literal, str, MATCH_NONE, "ciao = 4", T_NOVALUE,
                   &t);

    char buf2[] = "  \"ciao   \"";
    str = buf2;
    t = NULL;
    run_token_test("match_string_literal(  \"ciao   \")", match_string_literal, str, MATCH_FULL, "", L_S, &t);

    char buf3[] = "  ciao   \"";
    str = buf3;
    t = NULL;
    run_token_test("match_string_literal(  ciao   \")", match_string_literal, str, MATCH_NONE, "ciao   \"", T_NOVALUE,
                   &t);

    char buf4[] = "  \"ciao   ";
    str = buf4;
    t = NULL;
    run_token_test("match_string_literal(  \"ciao   )", match_string_literal, str, MATCH_PARTIAL, "\"ciao   ",
                   T_NOVALUE, &t);

    str = "  a5";
    t = NULL;
    run_token_test("match_integer_literal(  a5)", match_integer_literal, str, MATCH_NONE, "a5", T_NOVALUE, &t);

    str = "  3192384";
    t = NULL;
    run_token_test("match_integer_literal(  3192384)", match_integer_literal, str, MATCH_PARTIAL, "3192384", T_NOVALUE,
                   &t);

    str = "  100;";
    t = NULL;
    run_token_test("match_integer_literal(  100;)", match_integer_literal, str, MATCH_FULL, ";", L_I, &t);

    str = "  100 ";
    t = NULL;
    run_token_test("match_integer_literal(  100 )", match_integer_literal, str, MATCH_FULL, "", L_I, &t);

    str = "  100+";
    t = NULL;
    run_token_test("match_integer_literal(  100+)", match_integer_literal, str, MATCH_FULL, "+", L_I, &t);

    str = "123456789123456789 ";
    t = NULL;
    run_token_test("match_integer_literal(123456789123456789 )", match_integer_literal, str, MATCH_FULL, "", L_I, &t);

    // ================== IDENTIFIERS ==================
    str = "  abc";
    t = NULL;
    run_token_test("match_identifier(  abc)", match_identifier, str, MATCH_PARTIAL, "abc", T_NOVALUE, &t);

    str = "  x12_ ";
    t = NULL;
    run_token_test("match_identifier(  x12_ )", match_identifier, str, MATCH_FULL, "", ID, &t);

    str = "  = 5";
    t = NULL;
    run_token_test("match_identifier(  = 5)", match_identifier, str, MATCH_NONE, "= 5", T_NOVALUE, &t);

    str = "  100+";
    t = NULL;
    run_token_test("match_identifier(  100+)", match_identifier, str, MATCH_NONE, "100+", T_NOVALUE, &t);

    str = "  _45";
    t = NULL;
    run_token_test("match_identifier(  _45)", match_identifier, str, MATCH_PARTIAL, "_45", T_NOVALUE, &t);

    str = "int_5";
    t = NULL;
    run_token_test("match_identifier(int_5)", match_identifier, str, MATCH_PARTIAL, "int_5", T_NOVALUE, &t);
}