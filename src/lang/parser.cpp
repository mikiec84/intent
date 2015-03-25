#include "lang/lexer.h"
#include "lang/parser.h"

using namespace intent::core::text;

using std::string;
using std::stringstream;

namespace intent {
namespace lang {


struct parser::impl_t {
    lexer lex;

    impl_t(str_view const & txt) : lex(txt) {
    }
};


parser::parser(char const * begin, char const * end) : parser(str_view(begin, end)) {
}


parser::parser(char const * begin, size_t length) : parser(str_view(begin, length)) {
}


parser::parser(str_view const & txt) :
    impl(new impl_t(txt)) {
}


parser::~parser() {
    delete impl;
}


} // end namespace lang
} // end namespace intent
