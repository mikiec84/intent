#include <stack>

#include "core/work/progress_tracker.h"
#include "lang/ast/token_node.h"
#include "lang/lexer.h"
#include "lang/parser.h"

using namespace intent::core::text;
using namespace intent::lang::ast;

using std::string;
using std::stringstream;

using intent::core::work::work_type;
using intent::core::work::progress_tracker;


namespace intent {
namespace lang {

typedef node::handle_t node_handle;


struct parser::impl_t {
	lexer lex;
	progress_tracker * progress;

	impl_t(str_view const & txt) : lex(txt), progress(nullptr) {
	}
};


parser::parser(char const * begin, char const * end) : parser(str_view(begin, end)) {
}


parser::parser(char const * begin, size_t length) : parser(str_view(begin, length)) {
}


parser::parser(str_view const & txt) :
	impl(new impl_t(txt)) {
}


node_handle parser::build_ast(progress_tracker * p) {
	impl->progress = p;
	if (p) {
		//p->expect_work(work_type::read_items, 1);
	}

	node_handle root;
	typedef std::stack<node_handle> node_stack;
	node_stack pending;

	lexer::iterator lex_it = impl->lex.begin();
	while (lex_it) {
/*
		"car: vehicle\n"
		"    properties:\n"
		"        - make: +< name\n"
		"        - price: double\n";
*/
		ast::node * nodeptr = nullptr;
		auto tt = lex_it->type;
		switch (tt) {
		case tt_indent:
		case tt_dedent:
		case tt_noun:
		case tt_verb:
			nodeptr = new token_node(nullptr, *lex_it);
			break;
		case tt_if:
		case tt_else:
		case tt_operator_colon:
			break;
		default:
			break;
		}
		++lex_it;
	}
	return root;
}


parser::~parser() {
	delete impl;
}


} // end namespace lang
} // end namespace intent
