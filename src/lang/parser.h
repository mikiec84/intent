#ifndef _d1c7ed60cc3d4a079e7232f44d26a225
#define _d1c7ed60cc3d4a079e7232f44d26a225

#include <cstddef>
#include <memory>

#include "core/text/str_view.h"
#include "lang/ast/node.h"

namespace intent {

namespace core {
namespace work {
	class progress_tracker;
}}

namespace lang {

/**
 * Parse a chunk of intent code.
 *
 * This is a hand-written, high-performance LR parser.
 */
class parser {
	struct impl_t;
	impl_t * impl;

public:
	parser(char const * begin, char const * end);
	parser(char const * begin, size_t length);
	parser(core::text::str_view const & txt);
	~parser();

	/**
	 * Convert raw text into an abstract syntax tree.
	 *
	 * @return root of ast
	 */
	ast::node::handle_t build_ast(core::work::progress_tracker * = nullptr);
};

}} // end namespace

#endif // sentry
