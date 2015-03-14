#ifndef _44a73bae58554c0f9b1c3e4aa41fabf8
#define _44a73bae58554c0f9b1c3e4aa41fabf8

#include "lang/lexer.h"

namespace intent {
namespace lang {

/**
 * Behaves like a pointer to a token.
 */
class lexer::iterator: public std::iterator<std::input_iterator_tag, token> {
public:
    iterator() : lex(nullptr) {}
    bool operator ==(iterator const & rhs) const { return lex == rhs.lex; }
    bool operator !=(iterator const & rhs) const { return !(*this == rhs); }
    token & operator *() { return lex->t; }
    token * operator ->() { return &(lex->t); }
    operator bool() const { return lex != nullptr; }
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

}} // end namespace

#endif
