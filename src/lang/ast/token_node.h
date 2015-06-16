#ifndef _773e5c656fdb4eabb948204cfdca0553
#define _773e5c656fdb4eabb948204cfdca0553

#include "lang/ast/node.h"


namespace intent {
namespace lang {
namespace ast {


/**
 * A node that's wholly equivalent to a lexical token. These are terminal
 * symbols in the grammar.
 */
class token_node : public node {
	token t;
public:
	token_node(node * parent, token const & _t) : node(parent), t(_t) {}
	virtual ~token_node() {}

	virtual node_type get_type() const { return static_cast<node_type>(t.type); }
	bool is_token() const { return true; }
};


}}} // end namespace

#endif // sentry
