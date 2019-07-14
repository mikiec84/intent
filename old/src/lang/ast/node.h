#ifndef _5650cbbd17644dc0946a1cdbd05a2400
#define _5650cbbd17644dc0946a1cdbd05a2400

#include <cstdint>
#include <limits>
#include <memory>

#include "lang/token.h"
#include "lang/ast/node_type.h"

static constexpr auto in_any_generation = std::numeric_limits<unsigned>::max();


namespace intent {
namespace lang {
namespace ast {


/**
 * A node is the core building block of all parsed data. An abstract syntax tree
 * consists of one or more nodes of different types, arranged in a containment
 * hierarchy.
 *
 * Nodes own their children; children maintain a back ptr to their parent, but
 * this ptr should be treated like a weak reference.
 */
class node {
	node * parent;
	node ** children;
	uint32_t child_count;
	uint32_t allocated_child_count;
public:
	typedef std::unique_ptr<node> handle_t;

	// Not copyable.
	node(node const &) = delete;
	node & operator =(node const &) = delete;

	node(node * parent = nullptr);
	virtual ~node();

	node * get_parent();
	node const * get_parent() const;
	void set_parent(node *);

	/**
	 * Decide whether candidate is an ancestor of this node, within N generations.
	 *
	 * @param candidate The node of interest--a potential ancestor.
	 * @param within_n_generations How many generations back to look. Must be
	 *     at least 1.
	 * @return How many generations separate this node from its ancestor--or 0
	 *     if there is no link.
	 */
	unsigned has_ancestor(node * candidate, unsigned within_n_generations = in_any_generation);

	/**
	 * Decide whether candidate is a descendant of this node, within N generations.
	 * @param candidate The node of interest--a potential ancestor.
	 * @param within_n_generations How many generations back to look. Must be
	 *     at least 1.
	 */
	unsigned has_descendant(node * candidate, unsigned within_n_generations = in_any_generation);

	node * get_root();
	node const * get_root() const;

	virtual node_type get_type() const { return static_cast<node_type>(0); }
	bool is_token() const { return false; }

	void add_child(node *);
	uint32_t get_child_count() const;

	node * get_child(uint32_t i);
	node const * get_child(uint32_t i) const;
};


}}} // end namespace

#include "lang/ast/node-inline.h"

#endif // sentry
