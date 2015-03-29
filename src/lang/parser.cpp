#include "lang/lexer.h"
#include "lang/parser.h"
#include "core/work/progress_tracker.h"

using namespace intent::core::text;

using std::string;
using std::stringstream;

using intent::core::work::work_type;
using intent::core::work::progress_tracker;

namespace intent {
namespace lang {


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


ast::node::handle_t parser::build_ast(progress_tracker * p) {
    impl->progress = p;
    if (p) {
        p->expect_work(work_type::read_items, 1);
    }
    return nullptr;
}


parser::~parser() {
    delete impl;
}


} // end namespace lang
} // end namespace intent
