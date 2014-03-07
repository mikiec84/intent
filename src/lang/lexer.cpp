#include <cstring>

#include "lang/lexer.h"

namespace intent {
namespace lang {

lexer::lexer(char const * txt) : txt(txt), txt_end(strchr(txt, 0)) {
}

lexer::lexer(char const * txt, char const * txt_end) : txt(txt), txt_end(txt_end) {
}

bool lexer::advance() {
    return false;
}

bool lexer::read_number() {
    return nullptr;
}

} // end namespace lang
} // end namespace intent


