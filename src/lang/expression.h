#ifndef intent_lang_expression_h
#define intent_lang_expression_h

#include "lang/token.h"

namespace intent {
namespace lang {

/**
 * An expression is anything that evaluates to a value.
 */
struct expression
{
    virtual ~expression();
    virtual token const & first_token() const;
    virtual token const & last_token() const;
};

struct literal_expression: public expression {
    token content;
    literal_expression(token const & t) : content(t) {}
    virtual ~literal_expression() {}
};

struct grouped_expression: public expression {
    expression const & inner;
    grouped_expression(expression const & e) : inner(e) {}
    virtual ~grouped_expression() {}
};

#if 0
struct binary_expression: public expression {
    expression & first_operand;
    token_type binary_operator;
    expression & second_operand;
    virtual ~binary_expression() {}
};

struct unary_expression: public expression {
    expression & operand;
    token_type unary_operator;
    virtual ~unary_expression() {}
};

struct ternary_expression: public expression {
    expression & first_operand;
    expression & second_operand;
    expression & third_operand;
    virtual ~ternary_expression() {}
};

#endif

}} // end namespace

#endif // SYNTAX_ERROR_H
