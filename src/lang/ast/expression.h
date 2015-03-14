#ifndef _fe11fee2c3e548588caa3510c79a141c
#define _fe11fee2c3e548588caa3510c79a141c

#include "lang/token.h"
#include "lang/ast/node.h"

namespace intent {
namespace lang {
namespace ast {

/**
 * An expression is anything that evaluates to a value.
 */
class expression: public node {
public:
    expression(node * parent);
    virtual ~expression();
    virtual token const & first_token() const;
    virtual token const & last_token() const;
};

class literal_expression: public expression {
public:
    literal_expression(node * parent);
    token content;
    literal_expression(token const & t);
    virtual ~literal_expression() {}
};

class grouped_expression: public expression {
public:
    grouped_expression(node * parent);
    expression const & inner;
    grouped_expression(expression const & e) ;
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


}}} // end namespace

#endif
