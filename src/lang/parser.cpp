#include "lang/parser.h"

using namespace intent::core::text;

using std::string;
using std::stringstream;

namespace intent {
namespace lang {


struct parser::impl_t {
    lexer lex;
};


parser::parser(char const * begin, char const * end) : parser(str_view(begin, end)) {
}


parser::parser(char const * begin, size_t length) : parser(str_view(begin, length)) {
}


parser::parser(str_view const & _txt) :
    txt(_txt), line_begin(_txt.begin), p(_txt.begin),
    inconsistent_indent(nullptr), line_number(0), total_indent_width(0),
    indent_dedent_delta(0), last_stack_insert_idx(NO_INDENT_YET) {



} // end namespace lang
} // end namespace intent
