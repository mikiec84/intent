#ifndef _277502315e4b4c40b2b3f7a66c9c35f3
#define _277502315e4b4c40b2b3f7a66c9c35f3

namespace intent {
namespace lang {
namespace ast {


static constexpr auto non_token_mask = 0x10000;


/**
 * A node_type is a numeric identifier for a rule in our grammar.
 *
 * Some node types are simply token types; these are terminal grammar rules
 * that describe particular outputs from the lexer. Others are higher-level
 * constructs.
 */
enum node_type {

	#define tuple(number, name, example, comment) \
		nt_token_##name = number,
	#include "lang/token_type.tuples"

	#define tuple(number, precedence, associativity, name, example, comment) \
		nt_operator_##name = 0x8000 | number,
	#include "lang/operator.tuples"

	#define tuple(number, name) \
		nt_##name = non_token_mask | number,
	#include "lang/ast/node_type.tuples"
};


inline bool node_type_is_token(node_type n) {
	return (static_cast<int>(n) | non_token_mask) == 0;
}


}}} // end namespace

#endif // sentry
