#ifndef intent_lang_token_type_h
#define intent_lang_token_type_h

namespace intent {
namespace lang {

/**
 * When two operators have the same precedence, the expression is evaluated
 * according to its associativity. For example x = y = z = 17 is treated as
 * x = (y = (z = 17)), leaving all three variables with the value 17, since
 * the = operator has right-to-left associativity (and an assignment statement
 * evaluates to the value on the right-hand side). On the other hand,
 * 72 / 2 / 3 is treated as (72 / 2) / 3 since the / operator has left-to-
 * right associativity.
 */
enum operator_associativity {
    oa_left_to_right,
    oa_right_to_left
};

/**
 * Uniquely identify the semantics of a token.
 */
enum token_type {

    #define TUPLE(number, name, example, comment) \
        tt_##name = number,
    #include "lang/token_type_tuples.h"

    #define TUPLE(number, precedence, associativity, name, example, comment) \
        tt_operator_##name = 0x8000 | number,
    #include "lang/operator_tuples.h"
};

bool is_comment(token_type);
bool is_literal(token_type);
bool is_number_literal(token_type);
bool is_string_literal(token_type);
bool is_date_literal(token_type);
bool is_operator(token_type);

size_t get_token_type_count();
token_type get_token_type_by_index(size_t i);
token_type get_token_type_from_name(char const * name);
char const * get_token_type_name(token_type);
char const * get_token_type_example(token_type);
char const * get_token_type_comment(token_type);
int get_operator_precedence(token_type);
operator_associativity get_operator_associativity(token_type);

}} // end namespace

#include "lang/token_type-inline.h"

#endif // sentry
