#ifndef _ec56261ea62b4f4ba1594eccf05494ff
#define _ec56261ea62b4f4ba1594eccf05494ff

#ifndef _5e2636d642be46bc91ebd85c04781b80
#include "lang/lexer.h"
#endif

namespace intent {
namespace lang {

class lexer::iterator: public std::iterator<std::input_iterator_tag, token> {
public:
    iterator() : lex(nullptr) {}
    bool operator ==(iterator const & rhs) const { return lex == rhs.lex; }
    bool operator !=(iterator const & rhs) const { return !(*this == rhs); }
    token & operator *() { return lex->t; }
    token * operator ->() { return &(lex->t); }
    iterator & operator++() { if (!lex->advance()) lex = nullptr; return *this; }

private:
    friend class lexer;
    iterator(lexer * lex) : lex(lex) {}
    lexer * lex;
};

inline lexer::iterator lexer::begin() {
    return txt ? iterator(this) : iterator();
}

inline lexer::iterator lexer::end() const {
    return iterator();
}

inline char lexer::get_indent_char() const {
    return indent_stack[0];
}

} // end namespace lang
} // end namespace intent

#endif
